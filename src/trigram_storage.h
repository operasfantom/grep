#ifndef TRIGRAM_STORAGE_H
#define TRIGRAM_STORAGE_H

#include <QByteArray>
#include <QHash>
#include <QVector>

using Trigram = QByteArray;

class trigram_storage
{
    static const int ALPHA = 36;
    static const int TOTAL_TRIGRAM_COUNT = ALPHA * ALPHA * ALPHA;

    QHash<QString, qint64> occurrences_number;

    void index_file(QString file_path);

    void add_trigram(QString trigram);
public:
    static const int T = 3;

    trigram_storage();

    void add_data(QString string);

    void add_data(QString first_array, QString second_array);

    bool contains_substring(QString string) const;

    bool is_text() const;
};

#endif // TRIGRAM_STORAGE_H
