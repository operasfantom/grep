#include "directory_controller.h"

#include <QDirIterator>
#include <QHash>
#include <QThread>

#include "file_iterator.h"
#include "KMP.h"

namespace {
	class Runnable final : public QRunnable {
		std::function<void()> f;
	public:
		template <typename F>
		Runnable(F f) : f(f) {}

		void run() override {
			f();
		};
	};
}

directory_controller::directory_controller(QObject* parent) : QObject(parent) {
	buffer.resize(2);
	buffer[0].resize(BUFFER_SIZE);
	buffer[1].resize(BUFFER_SIZE);

	scanning_thread_pool.setMaxThreadCount(QThread::idealThreadCount() * 10);
	/*connect(&watcher, &QFileSystemWatcher::directoryChanged, [this](const QString& path) {
		this->file_changed(path);
	});*/
}

void directory_controller::set_directory(QString directory_name) {
	directory = QDir(directory_name);
	directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
	// directory.setSorting(QDir::Size | QDir::Reversed);

	scan_directory(false);
}

void directory_controller::add_path(QString dir_info) {
	// watcher.addPath(dir_info);
}

void directory_controller::add_paths() {
	QDirIterator it(directory.path(), QDir::Dirs, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		add_path(it.next());
	}
}

void directory_controller::clear_storage() {
	// QStringList remove_paths = watcher.directories();
	// QStringList failed_list = watcher.removePaths(remove_paths); //todo dtor
	storage_by_file.clear();
}

void directory_controller::process_indexed_file(QString file_path, trigram_storage&& storage) {
	storage_by_file.insert(file_path, std::make_shared<trigram_storage>(std::move(storage)));
}

void directory_controller::add_file(QString file_path) {
	QFile file(file_path);
	if (file.open(QFile::ReadOnly)) {
		trigram_storage storage;
		int odd = 0;
		bool is_first_block = true;
		file_iterator iterator(file_path);
		try {
			while (iterator.hasNext()) {
				buffer[odd] = iterator.next();
				// buffer[odd] = buffer[odd].toLower();
				// scanning_thread_pool.start(new Runnable([is_first_block, &storage, buffer = this->buffer, odd]() mutable {
					if (is_first_block) {
						storage.add_data(buffer[odd]);
					}
					else {
						storage.add_data(buffer[odd ^ 1], buffer[odd]);
					}
				// }));
				odd ^= 1;
				is_first_block = false;
			}
			// scanning_thread_pool.waitForDone();
			if (storage.is_text()) {
				process_indexed_file(file_path, std::move(storage));
			}
		}
		catch (...) {}
	}
}

void directory_controller::remove_file(QString file_path) {
	storage_by_file.remove(file_path);
}

void directory_controller::scan_directory0() {
	clear_storage();
	if (directory.exists()) {
		if (state == State::COMPLETED) {
			state = State::IN_PROCESS;
			QDirIterator it(directory.path(), QDir::Files, QDirIterator::Subdirectories);
			// int i = 0;
			while (it.hasNext() && state == State::IN_PROCESS) {
				add_file(it.next());
				// emit set_progress(100.0 * (++i) / total_files_count);
			}
			if (state == State::IN_PROCESS) {
				state = State::COMPLETED;
			}
		}
	}
}

void directory_controller::scan_directory(bool sync) {
	auto f = [this]() {
		try {
			scan_directory0();

			bool success = (state == State::COMPLETED);
			state = State::COMPLETED;
			emit finished_scanning(success);
		}
		catch (...) {
			state = State::COMPLETED;
			emit finished_scanning(false);
		}
	};
	if (sync) {
		f();
	}
	else {
		scan_thread = QThread::create([this, f]() {
			f();
		});
		scan_thread->start();
	}
}

bool directory_controller::search_substring_in_file(QString file_path, QString substring) const {
	KMP kmp(substring);

	file_iterator iterator(file_path);
	while (iterator.hasNext()) {
		QString string = iterator.next();
		for (auto c : string) {
			if (kmp.load(c)) {
				return true;
			}
		}
	}
	return false;
}

bool directory_controller::contains_substring(QString file_path, trigram_storage const& storage, QString string) const {
	if (!storage.contains_substring(string)) {
		return false;
	}
	return search_substring_in_file(file_path, string);
}

void directory_controller::search_substring(QString string) {
	search_thread = QThread::create([this, string]() mutable {
		if (state == State::COMPLETED) {
			state = State::IN_PROCESS;
			try {
				//string = string.toLower();
				int i = 0;
				for (auto it = storage_by_file.begin(); it != storage_by_file.end() && state == State::IN_PROCESS; ++it
				) {
					if (contains_substring(it.key(), *(it.value()), string)) {
						emit send_search_result(it.key());
					}
					emit set_progress(100.0 * (++i) / storage_by_file.size());
				}

				bool success = state == State::IN_PROCESS;
				state = State::COMPLETED;;
				emit finished_searching(success);
			}
			catch (...) {
				state = State::COMPLETED;
				emit finished_searching(false);
			}
		}
	});
	search_thread->start();
}

void directory_controller::cancel() {
	state = State::CANCELLED;
}

void directory_controller::file_changed(QString file_path) {
	QFile file(file_path);
	if (file.exists()) {
		add_file(file_path);
	}
	else {
		remove_file(file_path);
	}
}
