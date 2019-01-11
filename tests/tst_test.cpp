#include "tst_test.h"

test::test()
{

}

test::~test()
{

}

void test::create_file(QString data, int quantity)
{
    static int id;
    for (int i = 0; i < quantity; ++i) {
        QFile file(GENERATE_PATH.filePath(QString::number(++id)));
//        qDebug(QFileInfo(file).absoluteFilePath().toUtf8());
        if (file.open(QFile::ReadWrite)) {
            file.write(data.toUtf8());
            file.close();
        }
    }
}

void test::create_file_subdirectory(QString data, int quantity)
{
    static int subdirectory_id;
    static int id;
    QString relative_folder = "dir" + QString::number(++subdirectory_id);
    QDir relative_path = GENERATE_PATH.filePath(relative_folder);
    GENERATE_PATH.mkdir(relative_folder);
    for (int i = 0; i < quantity; ++i) {
        QFile file(relative_path.filePath(QString::number(++id)));
//        qDebug(QFileInfo(file).absoluteFilePath().toUtf8());
        if (file.open(QFile::ReadWrite)) {
            file.write(data.toUtf8());
            file.close();
        }
    }
}

void test::init()
{
    CURRENT_PATH.mkdir(GENERATE_FOLDER);

    controller.set_directory(GENERATE_PATH.path());

    connect(&controller, &directory_controller::send_duplicates_group, [&](QFileInfoList list) {
       group_sizes.insert(list.size());
    });
}

void test::cleanup()
{
    GENERATE_PATH.removeRecursively();

    group_sizes.clear();
}

void test::test_case_empty_folder()
{
    controller.scan_directory(true);

    QCOMPARE((QSet<int>{}), group_sizes);
}

QTEST_MAIN(test)

//#include "tst_test.moc"
