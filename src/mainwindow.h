#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "directory_controller.h"
#include "ui_mainwindow.h"

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class main_window : public QMainWindow {
Q_OBJECT
private:
	void show_message(QString string);

	bool validate_input(QString string);
public:
	explicit main_window(QWidget* parent = 0);
	~main_window();


	void search_substring(QString string);

	void disable_actions();
private slots:
	void select_directory();
	void show_about_dialog();
	void show_file(QString file_path);
	void finished_scanning(bool success);
	void finished_searching(bool success);

	void on_searchButton_clicked();

private:
	//    std::unique_ptr<Ui::MainWindow> ui;
	Ui::MainWindow* ui;

	directory_controller controller;

	//const QRegExp input_reg_exp = QRegExp("([a-z,0-9, ])");
};

#endif // MAINWINDOW_H
