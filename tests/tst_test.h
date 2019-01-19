#ifndef TST_TEST_H
#define TST_TEST_H

#include <QtTest>
#include <QWaitCondition>

#include "../src/directory_controller.h"

class test : public QObject {
Q_OBJECT

public:
	test();
	~test();
private:
	const QDir CURRENT_PATH = QDir::currentPath();
	const QString GENERATE_FOLDER = "GENERATE_PATH";
	QDir GENERATE_PATH = CURRENT_PATH.filePath(GENERATE_FOLDER);

	directory_controller controller;

	int found = 0;

	QMutex mutex;
	QWaitCondition cv;
	int expected = -1;
	QString last_file_name;

	void create_file(QString file_name, QString data);
	void create_file(QString data, int quantity = 1);
	void create_file_subdirectory(QString data, int quantity = 1);
	void remove_last_file();
	void run(QString string, int expected);
private slots:
	void init();
	void cleanup();
	void initTestCase();
	void cleanupTestCase();
	void test_case_empty_folder();
	void test_subdirectory();
	void test_one_letter();
	void test_case1();
	void test_case2();
	void test_case3();
	void test_case4();
	void test_bound();
	void test_watcher();
};

#endif // TST_TEST_H
