#include "tst_test.h"

#include <QMutexLocker>

test::test() {}

test::~test() {}

void test::create_file(QString file_name, QString data) {
	QFile file(file_name);
	Q_ASSERT(file.open(QFile::ReadWrite));
	file.write(data.toUtf8());
	file.close();

	last_file_name = file_name;
}

void test::create_file(QString data, int quantity) {
	static int id;
	for (int i = 0; i < quantity; ++i) {
		QString path = GENERATE_PATH.filePath(QString::number(++id));
		create_file(path, data);
	}
}

void test::create_file_subdirectory(QString data, int quantity) {
	static int subdirectory_id;
	static int id;
	QString relative_folder = "dir" + QString::number(++subdirectory_id);
	QDir relative_path = GENERATE_PATH.filePath(relative_folder);
	GENERATE_PATH.mkdir(relative_folder);
	for (int i = 0; i < quantity; ++i) {
		QString path = relative_path.filePath(QString::number(++id));
		create_file(path, data);
	}
}

void test::remove_last_file() {
	QFile file(last_file_name);
	Q_ASSERT(file.remove());
}

void test::run(QString string, int expected) {
	this->expected = expected;
	connect(&controller, &directory_controller::finished_scanning,
	        [this, string](bool success) {
		        QCOMPARE(success, true);

		        connect(&controller, &directory_controller::finished_searching,
		                [this](bool success) {
			                QCOMPARE(success, true);
			                QCOMPARE(found, this->expected);
			                cv.notify_all();
		                });

		        controller.search_substring(string);
	        });

	controller.set_directory(GENERATE_FOLDER);
	QMutexLocker locker(&mutex);
	cv.wait(&mutex);
}

void test::init() {
	Q_ASSERT(GENERATE_PATH.removeRecursively());
	Q_ASSERT(CURRENT_PATH.mkdir(GENERATE_FOLDER));


	connect(&controller, &directory_controller::send_search_result, [&](QString file_path) {
		++found;
	});
}

void test::cleanup() {	
	// Q_ASSERT(GENERATE_PATH.removeRecursively());
	GENERATE_PATH.removeRecursively();

	found = 0;
	Q_ASSERT(controller.disconnect());
}

void test::initTestCase() {}

void test::cleanupTestCase() { }

void test::test_case_empty_folder() {
	run("", 0);
}

void test::test_subdirectory() {
	create_file("hello", 2);
	create_file_subdirectory("yellow", 2);
	run("ello", 4);
}

void test::test_one_letter() {
	create_file("abab");
	create_file("abacaba");
	create_file("hello");
	run("a", 2);
}

void test::test_case1() {
	create_file("abacaba");
	run("abacaba", 1);
}

void test::test_case2() {
	create_file("abacaba", 3);
	run("a", 3);
}

void test::test_case3() {
	create_file("abcdefghijkl");
	run("abcd", 1);
}

void test::test_case4() {
	create_file("abcdefghijkl");
	run("ijkl", 1);
}

void test::test_bound() {
	QString string = QString(4 * 1024 * 1024, 'a') + QString("bc");
	create_file(string, 1);
	run("abc", 1);
}

void test::test_watcher() {
	const int N = 3;
	create_file("abacaba", 3);
	run("ab", 3);
	found = 0;
	remove_last_file();
	run("ab", 2);
}

QTEST_MAIN(test)

//#include "tst_test.moc"
