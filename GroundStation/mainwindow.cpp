#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //把串口m托管给线程n
    m.moveToThread(&n);
    n.start();

}

MainWindow::~MainWindow()
{
    n.quit();
    n.wait();
    delete ui;
}

void MainWindow::main_test_slot()
{

}


void MainWindow::on_pushButton_clicked()
{
    qDebug() << "我们";
}
