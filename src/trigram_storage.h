#ifndef TRIGRAM_STORAGE_H
#define TRIGRAM_STORAGE_H

#include <QByteArray>
#include <QHash>
#include <QVector>

#include <memory>

using Trigram = QByteArray;

class trigram_storage
{
    static const int ALPHA = 36;
    static const int TOTAL_TRIGRAM_COUNT = ALPHA * ALPHA * ALPHA;

    QHash<QString, qint64> occurrences_number;

    void index_file(QString file_path);

    void add_trigram(QString trigram);

	static QStringList split_string_for_search(QString string);

	static QStringList split_string_for_storage(QString string);
public:
    static const int T;

    trigram_storage();

    trigram_storage(trigram_storage &&) noexcept = default;

    void add_data(QString string);

    void add_data(QString first_array, QString second_array);

    bool includes(trigram_storage const& other) const;

    bool contains_substring(QString string) const;

    bool is_text() const;
private:
	QString last_search_string;
	mutable std::unique_ptr<trigram_storage> cache;
};

#endif // TRIGRAM_STORAGE_H
