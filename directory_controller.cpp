#include "directory_controller.h"

#include <QDirIterator>

directory_controller::directory_controller(QObject *parent) : QObject(parent) {
    buffer[0].resize(BUFFER_SIZE);
    buffer[1].resize(BUFFER_SIZE);
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
    while ((bytes_read = file.read(buffer[odd].data(), BUFFER_SIZE)) > 0) {
        if (is_first_block) {
            storage.add_data(buffer[odd], bytes_read);
        } else {
            storage.add_data(buffer[odd ^ 1], BUFFER_SIZE, buffer[odd], bytes_read);
        }
        odd ^= 1;
        is_first_block = false;
    }
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

void directory_controller::search_substring(QString string)
{
    storage.search_substring(string);
}
