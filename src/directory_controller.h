#ifndef DIRECTORY_CONTROLLER_H
#define DIRECTORY_CONTROLLER_H

#include <QDir>
#include <QFileSystemWatcher>
#include <QHash>

#include <thread>

#include "trigram_storage.h"
#include <qthreadpool.h>

class directory_controller final : public QObject {
Q_OBJECT
private:
	enum class State {
		CANCELLED,
		IN_PROCESS,
		COMPLETED
	};

	void add_path(QString dir_info);

	void add_paths();

	void clear_storage();

	void process_indexed_file(QString file_path, trigram_storage&& storage);

	void add_file(QString file_path);

	void remove_file(QString file_path);

	void scan_directory0();

	void scan_directory(bool sync);

	bool search_substring_in_file(QString file_path, QString substring) const;

	bool contains_substring(QString file_path, trigram_storage const& storage, QString string) const;
public:
	std::atomic<State> state = State::COMPLETED;

	explicit directory_controller(QObject* parent = nullptr);

	virtual ~directory_controller() = default;

	void set_directory(QString directory_name);

	void search_substring(QString string);

	void cancel();
signals:
	void finished_scanning(bool success);

	void finished_searching(bool success);

	void set_progress(int value);

	void send_search_result(QString file_path);
private slots:
	void file_changed(QString file_path);

private:
	QDir directory;

	//    trigram_storage occurrences_number;

	//QFileSystemWatcher watcher;

	const int BUFFER_SIZE = 4 * 1024 * 1024;

	mutable QVector<QString> buffer;

	QHash<QString, std::shared_ptr<trigram_storage>> storage_by_file;

	QThread* search_thread;

	QThread* scan_thread;

	QThreadPool scanning_thread_pool;
};

#endif // DIRECTORY_CONTROLLER_H
