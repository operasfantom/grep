#include "directory_controller.h"

#include <QDirIterator>
#include <QSet>
#include <QHash>

directory_controller::directory_controller(QObject *parent) : QObject(parent) {
    buffer[0].resize(BUFFER_SIZE);
    buffer[1].resize(BUFFER_SIZE);

    connect(&watcher, SIGNAL(fileChanged(QString)), this, SLOT(file_changed(QString)));
}

void directory_controller::set_directory(QString directory_name)
{
    directory = QDir(directory_name);
    directory.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    directory.setSorting(QDir::Size | QDir::Reversed);

    scan_directory(false);
}

void directory_controller::add_path(QString dir_info)
{
    watcher.addPath(dir_info);
}

void directory_controller::add_paths()
{
    QDirIterator it(directory.path(), QDir::Dirs, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        add_path(it.next());
    }
}

void directory_controller::clear_storage() {

}

void directory_controller::index_file(QString file_path)
{
    QFile file(file_path);
    int bytes_read = 0;
    int odd = 0;
    bool is_first_block = true;
    trigram_storage storage;
    while ((bytes_read = file.read(buffer[odd].data(), BUFFER_SIZE)) > 0) {
        if (is_first_block) {
            storage.add_data(buffer[odd], bytes_read);
        } else {
            storage.add_data(buffer[odd ^ 1], BUFFER_SIZE, buffer[odd], bytes_read);
        }
        odd ^= 1;
        is_first_block = false;
    }
    if (storage.is_text()) {
//        emit indexed_file(storage);
    }
}

void directory_controller::remove_index(QString file_path)
{
    //todo
}

void directory_controller::scan_directory0() {
    clear_storage();
    if (directory.exists()) {
        QDirIterator it(directory.path(), QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            index_file(it.next());
        }
    }
}

void directory_controller::scan_directory(bool sync)
{
    if (sync) {
        try {
            scan_directory0();
            emit finished(true);
        } catch (...) {
            emit finished(false);
        }
    } else {
        if (scan_thread.joinable()) {
            scan_thread.join();
        }
        scan_thread = std::thread([this]() {
            try {
                scan_directory0();
                add_paths();
                emit finished(true);
            } catch (...) {
                emit finished(false);
            }
        });
    }
}

void directory_controller::search_substring0(QFileInfo file_info)
{
    QFile file(file_info.absoluteFilePath());
    //todo search in file
}

void directory_controller::search_substring(QString string)
{
    QByteArray array = string.toUtf8();
    QSet<QString> set;
    for (Trigram t : trigram_storage::split(array)) {
        set.intersect(files_by_trigram[t]);
    }
    for (auto file_path : set) {
        search_substring0(file_path);
    }
}

void directory_controller::file_changed(QString file_path)
{
    QFile file(file_path);
    if (file.exists()) {
        index_file(file_path);
    } else {
        remove_index(file_path);
    }
}
