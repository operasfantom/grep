#ifndef TRIGRAM_STORAGE_H
#define TRIGRAM_STORAGE_H

#include <QByteArray>
#include <QHash>

class trigram_storage
{
    QHash<QByteArray, qint64> trigrams;

    void index_file(QString file_path);

    void add_trigram(QByteArray trigram);
public:
    trigram_storage();

    void add_data(QByteArray array, int size);

    void add_data(QByteArray first_array, int first_size, QByteArray second_array, int second_size);

    void search_substring(QString string);
};

#endif // TRIGRAM_STORAGE_H
