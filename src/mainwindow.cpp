#include "mainwindow.h"

#include <QCommonStyle>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

#include <iostream>

void main_window::show_message(QString string) {
	ui->messageLog->appendPlainText(string);
}

bool main_window::validate_input(QString string) {
	QRegExp input_reg_exp("([a-z,A-Z,0-9]+)");
	if (string.isEmpty()) {
		show_message("input is empty!");
		return false;
	}
	if (!input_reg_exp.exactMatch(string)) {
		show_message("input should satisfy the following regexp: " + input_reg_exp.pattern());
		return false;
	}
	return true;
}

main_window::main_window(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, size(), qApp->desktop()->availableGeometry(this)));

    ui->treeWidget->header()->setSectionResizeMode(0, QHeaderView::Stretch);

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

	connect(ui->cancelButton, &QPushButton::clicked, &controller, &directory_controller::cancel);

	connect(&controller, &directory_controller::set_progress, ui->progressBar, &QProgressBar::setValue);

	ui->messageLog->setReadOnly(true);
//    scan_directory(QDir::homePath());
}

main_window::~main_window()
{
    delete ui;
}

void main_window::search_substring(QString string)
{
	if (!validate_input(string)) {
		return;
	}
	ui->treeWidget->clear();
	ui->searchButton->setEnabled(false);
	ui->cancelButton->setEnabled(true);
    controller.search_substring(string);
}

void main_window::disable_actions() {
	ui->actionScan_Directory->setEnabled(false);
}

void main_window::select_directory()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory for Scanning",
                                                    QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (dir.isEmpty()) {
		return;
	}
	setWindowTitle(QString("Directory Content - %1").arg(dir));
	disable_actions();

	ui->treeWidget->clear();
	ui->cancelButton->setEnabled(true);
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
	
	if (success) {
		ui->lineEdit->setEnabled(true);
		ui->searchButton->setEnabled(true);
	} else {
		show_message("scanning was interrupted");
	}

	ui->cancelButton->setEnabled(false);
}

void main_window::finished_searching(bool success) {
	ui->lineEdit->setEnabled(true);
	ui->searchButton->setEnabled(true);

	if (!success) {
		show_message("searching was interrupted");
	}

	ui->cancelButton->setEnabled(false);
}

void main_window::on_searchButton_clicked()
{
    search_substring(ui->lineEdit->text());
}
