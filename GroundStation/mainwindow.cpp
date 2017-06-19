#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //把串口m托管给线程n
    MyCom.moveToThread(&MyComThread);
    MyComThread.start();

    //更新串口列表
    ui->comboBox_PortName->addItems(MyCom.SerialPort_Get_Port_List());

}

MainWindow::~MainWindow()
{
    MyComThread.quit();   //结束串口线程
    MyComThread.wait();   //等待线程完全结束
    delete ui;
}

void MainWindow::main_test_slot()
{

}

void MainWindow::on_pushButton_GetPort_clicked()
{
    ui->comboBox_PortName->clear();
    ui->comboBox_PortName->addItems(MyCom.SerialPort_Get_Port_List());
}

void MainWindow::on_pushButton_OpenPort_clicked()
{
    QString PortName = ui->comboBox_PortName->currentText();
    int Baud = ui->comboBox_BaudRate->currentText().toInt();

    if(MyCom.SerialPort_Open(PortName,Baud))
    {
        qDebug() << "串口打开成功，串口号：" << PortName << "，波特率：" << Baud;
    }
}
