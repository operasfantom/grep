#include "trigram_storage.h"

#include <QString>
#include <algorithm>

void trigram_storage::add_trigram(QString trigram)
{
    ++occurrences_number[trigram];
}

trigram_storage::trigram_storage()
{
}

void trigram_storage::add_data(QString string)
{
    for (int i = 0; i + T <= string.size(); ++i) {
        add_trigram(string.mid(i, 3));
    }
	for (int i = 1; i < T; ++i) {
		add_trigram(string.mid(0, i));
	}
}

void trigram_storage::add_data(QString first, QString second)
{
    for (int i = std::max(1, T - second.size()); i < T; ++i) {
        QString t = first.mid(first.size() - i, i) + second.mid(0, T - i);
        add_trigram(t);
    }
    add_data(second);
}

bool trigram_storage::contains_substring(QString string) const
{
    QHash<QString, qint64> temporary;
    for (int i = 0; i + T <= string.size(); ++i) {
        ++temporary[string.mid(i, 3)];
    }
	for (int i = 1; i < T; ++i) {
		++temporary[string.mid(0, i)];
	}
	for (auto it = temporary.begin(); it != temporary.end(); ++it) {
        if (occurrences_number[it.key()] < it.value()) {
			return false;
        }
    }
	return true;
}

bool trigram_storage::is_text() const
{
    return occurrences_number.size() <= TOTAL_TRIGRAM_COUNT;
}
