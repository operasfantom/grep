#ifndef TRIGRAM_STORAGE_H
#define TRIGRAM_STORAGE_H

#include <QByteArray>
#include <QHash>
#include <QVector>

using Trigram = QByteArray;

class trigram_storage
{
    QHash<QByteArray, qint64> trigrams;

    void index_file(QString file_path);

    void add_trigram(QByteArray trigram);
public:
    static const int T = 3;

    trigram_storage();

    static QVector<Trigram> split(QByteArray array);

    void add_data(QByteArray array, int size);

    void add_data(QByteArray first_array, int first_size, QByteArray second_array, int second_size);

    void search_substring(QString string);

    bool is_text() const;
};

#endif // TRIGRAM_STORAGE_H
