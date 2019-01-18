#include "mainwindow.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry(this)));

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->treeWidget->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    QCommonStyle style;
    ui->actionScan_Directory->setIcon(style.standardIcon(QCommonStyle::SP_DialogOpenButton));
    ui->actionExit->setIcon(style.standardIcon(QCommonStyle::SP_DialogCloseButton));
    ui->actionAbout->setIcon(style.standardIcon(QCommonStyle::SP_DialogHelpButton));

    connect(ui->actionScan_Directory, &QAction::triggered, this, &main_window::select_directory);
    connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
    connect(ui->actionAbout, &QAction::triggered, this, &main_window::show_about_dialog);
    connect(ui->actionAbout, &QAction::triggered, this, &main_window::show_about_dialog);

	connect(&controller, &directory_controller::send_search_result, this, &main_window::show_file);
	connect(&controller, &directory_controller::finished_scanning, this, &main_window::finished_scanning);
	connect(&controller, &directory_controller::finished_searching, this, &main_window::finished_searching);

	connect(&controller, &directory_controller::set_progress, ui->progressBar, &QProgressBar::setValue);
//    scan_directory(QDir::homePath());
}

main_window::~main_window()
{
    delete ui;
}

void main_window::search_substring(QString string)
{
	ui->treeWidget->clear();
	ui->searchButton->setEnabled(false);

    controller.search_substring(string);
}

void main_window::disable_actions() {
	ui->actionScan_Directory->setEnabled(false);
}

void main_window::select_directory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	setWindowTitle(QString("Directory Content - %1").arg(dir));
	disable_actions();

	ui->treeWidget->clear();
	ui->actionScan_Directory->setEnabled(false);
    controller.set_directory(dir);
}

void main_window::show_about_dialog()
{
    QMessageBox::aboutQt(this);
}

void main_window::show_file(QString file_path) {
	auto item = new QTreeWidgetItem(ui->treeWidget);
	item->setText(0, file_path);

	ui->treeWidget->addTopLevelItem(item);
}

void main_window::finished_scanning(bool success) {
	ui->actionScan_Directory->setEnabled(true);
	
	ui->lineEdit->setEnabled(false);
	ui->searchButton->setEnabled(false);
}

void main_window::finished_searching(bool success) {
	ui->searchButton->setEnabled(false);
}

void main_window::on_searchButton_clicked()
{
    search_substring(ui->lineEdit->text());
}
