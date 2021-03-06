#ifndef FILE_ITERATOR_H
#define FILE_ITERATOR_H

#include <QFile>
#include <QTextStream>

class file_iterator
{
	QFile file;
	QTextStream stream;
	static const qint64 BUFFER_SIZE = 4 * 1024 * 1024;
public:

	file_iterator(QString file_path);

	bool hasNext() const;

	QString next();

	~file_iterator();
	
};

#endif //FILE_ITERATOR_H