#include "trigram_storage.h"

#include <QString>
#include <algorithm>

void trigram_storage::add_trigram(QString trigram) {
	++occurrences_number[trigram];
}

QStringList trigram_storage::split_string_for_search(QString string) {
	QStringList result;
	if (string.size() >= trigram_storage::T) {
		for (int i = 0; i + T <= string.size(); ++i) {
			result.push_back(string.mid(i, T));
		}
	}
	else {
		for (int i = 1; i < std::min(T, string.size()); ++i) {
			result.push_back(string.mid(0, i));
		}
	}
	return result;
}

QStringList trigram_storage::split_string_for_storage(QString string) {
	QStringList result;
	for (int len = 1; len <= T; ++len) {
		for (int i = 0; i + len <= string.size(); ++i) {
			result.push_back(string.mid(i, len));
		}
	}
	return result;
}

const int trigram_storage::T = 3;

trigram_storage::trigram_storage() {}

void trigram_storage::add_data(QString string) {
	for (auto t : split_string_for_storage(string)) {
		add_trigram(t);
	}
}

void trigram_storage::add_data(QString first, QString second) {
	for (int i = std::max(1, T - second.size()); i < T; ++i) {
		QString t = first.mid(first.size() - i, i) + second.mid(0, T - i);
		add_trigram(t);
	}
	add_data(second);
}

bool trigram_storage::includes(trigram_storage const& other) const {
	for (auto it = other.occurrences_number.begin(); it != other.occurrences_number.end(); ++it) {
		auto key = it.key();
		if (occurrences_number[key] < it.value()) {
			return false;
		}
	}
	return true;
}

bool trigram_storage::contains_substring(QString string) const {
	if (string != last_search_string) {
		cache = std::make_unique<trigram_storage>();
		for (auto t : split_string_for_search(string)) {
			cache->add_trigram(t);
		}
	}
	return includes(*cache);
}

bool trigram_storage::is_text() const {
	return occurrences_number.size() <= TOTAL_TRIGRAM_COUNT;
}
