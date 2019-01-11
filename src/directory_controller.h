#ifndef DIRECTORY_CONTROLLER_H
#define DIRECTORY_CONTROLLER_H

#include <QDir>
#include <QFileSystemWatcher>
#include <QHash>

#include <thread>

#include "trigram_storage.h"

//namespace Ui {
//class directory_controller;
//}

class directory_controller : public QObject {
Q_OBJECT
private:
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
	explicit directory_controller(QObject* parent = nullptr);

	virtual ~directory_controller() = default;

	void set_directory(QString directory_name);

	void search_substring(QString string);
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

	QFileSystemWatcher watcher;

	const int BUFFER_SIZE = 4 * 1024 * 1024;

	QString buffer[2];

	QHash<QString, trigram_storage> storage_by_file;

	QThread* search_thread;

	QThread* scan_thread;
};

#endif // DIRECTORY_CONTROLLER_H
