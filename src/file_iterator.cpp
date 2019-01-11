#include "file_iterator.h"

file_iterator::file_iterator(QString file_path) : file(file_path), stream(&file) {
	file.open(QFile::ReadOnly);
	stream.setCodec("UTF-8");
}

bool file_iterator::hasNext() const {
	return !stream.atEnd();
}

QString file_iterator::next() {
	return stream.readLine(BUFFER_SIZE);
}

file_iterator::~file_iterator() {}
