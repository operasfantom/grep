#include "trigram_storage.h"

#include <QString>
#include <algorithm>

void trigram_storage::add_trigram(QByteArray trigram)
{
    ++trigrams[trigram];
}

trigram_storage::trigram_storage()
{
}

QVector<Trigram> trigram_storage::split(QByteArray array)
{
    QVector<Trigram> result;
    for (int i = 0; i + T <= array.size(); ++i) {
        result.push_back(array.mid(i, 3));
    }
    return result;
}

void trigram_storage::add_data(QByteArray array, int size)
{
    for (int i = 0; i + T <= size; ++i) {
        add_trigram(array.mid(i, 3));
    }
}

void trigram_storage::add_data(QByteArray first_array, int first_size, QByteArray second_array, int second_size)
{
    for (int i = std::max(1, T - second_size); i < T; ++i) {
        QByteArray t = first_array.mid(first_size - i, i) + second_array.mid(0, T - i);
        add_trigram(t);
    }
    add_data(second_array, second_size);
}

void trigram_storage::search_substring(QString string)
{
    QByteArray array = string.toUtf8();
    for (int i = 0; i + T <= string.size(); ++i) {
        add_trigram(array.mid(i, 3));
    }
}

bool trigram_storage::is_text() const
{

}
