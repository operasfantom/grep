#ifndef DIRECTORY_CONTROLLER_H
#define DIRECTORY_CONTROLLER_H

#include <QFileInfoList>
#include <QDir>
#include <QFileSystemWatcher>

#include <thread>
#include <functional>

#include "trigram_storage.h"

//namespace Ui {
//class directory_controller;
//}

class directory_controller : public QObject
{
    Q_OBJECT
private:
    void add_path(QString dir_info);

    void add_paths();

    void clear_storage();

    void index_file(QString file_path);

    void scan_directory0();

    void scan_directory(bool sync);
public:
    explicit directory_controller(QObject *parent = nullptr);

    virtual ~directory_controller() = default;

    void set_directory(QString directory_name);

    void search_substring(QString string);
signals:
    void finished(bool success);

    void send_search_results(QFileInfoList file_info_list);
private:
    QDir directory;

    trigram_storage storage;

    std::thread scan_thread;

    QFileSystemWatcher watcher;

    const int BUFFER_SIZE = 4 * 1024 * 1024;

    QByteArray buffer[2];
};

#endif // DIRECTORY_CONTROLLER_H
