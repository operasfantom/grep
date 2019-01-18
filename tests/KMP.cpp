#include "KMP.h"

int KMP::next(QChar c, int prev_f) {
	int j = (prev_f == n ? p[prev_f - 1] : prev_f);
	while (j > 0 && c != string[j]) {
		j = p[j - 1];
	}
	if (c == string[j]) {
		++j;
	}
	return j;
}

KMP::KMP(QString string) : string(string), n(string.length()), p(n) {
	p[0] = 0;
	for (int i = 1; i < n; i++) {
		p[i] = next(string.at(i).toLatin1(), p[i - 1]);
	}
}

bool KMP::load(QChar c) {
	j = next(c, j);
	return j == n;
}
