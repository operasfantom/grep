#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "directory_controller.h"
#include "ui_mainwindow.h"

#include <QMainWindow>
#include <memory>

namespace Ui {
class MainWindow;
}

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit main_window(QWidget *parent = 0);
    ~main_window();

     void search_substring(QString string);
private slots:
    void select_directory();
    void scan_directory(QString dir);
    void show_about_dialog();

    void on_searchButton_clicked();

private:
//    std::unique_ptr<Ui::MainWindow> ui;
    Ui::MainWindow * ui;

    directory_controller controller;
};

#endif // MAINWINDOW_H
