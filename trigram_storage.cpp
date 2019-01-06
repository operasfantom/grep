#include "trigram_storage.h"

#include <algorithm>

void trigram_storage::add_trigram(QByteArray trigram)
{
    ++trigrams[trigram];
}

trigram_storage::trigram_storage()
{
}

void trigram_storage::add_data(QByteArray array, int size)
{
    for (int i = 0; i + 3 <= size; ++i) {
        add_trigram(array.mid(i, 3));
    }
}

void trigram_storage::add_data(QByteArray first_array, int first_size, QByteArray second_array, int second_size)
{
    for (int i = std::max(1, 3 - second_size); i <= 2; ++i) {
        QByteArray t = first_array.mid(first_size - i, i) + second_array.mid(0, 3 - i);
        add_trigram(t);
    }
    add_data(second_array, second_size);
}

void trigram_storage::search_substring(QString string)
{

}
