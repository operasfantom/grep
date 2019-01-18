#ifndef KMP_H
#define KMP_H

#include <QVector>

class KMP {
	QString string;
	int n;
	QVector<int> p;

	int j = 0;

	int next(QChar c, int prev_f);

public:
	KMP(QString string);

	bool load(QChar c);
};

#endif //KMP_H