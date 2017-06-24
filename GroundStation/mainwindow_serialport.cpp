#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_pushButton_GetPort_clicked()
{
    ui->comboBox_PortName->clear();
    ui->comboBox_PortName->addItems(MyCom.SerialPort_Get_Port_List());
}

void MainWindow::on_pushButton_OpenPort_clicked()
{
    if(ui->pushButton_OpenPort->text() == "打开串口")
    {
        QString PortName = ui->comboBox_PortName->currentText();
        int Baud = ui->comboBox_BaudRate->currentText().toInt();

        MyCom.SerialPort_Open(PortName,Baud);
    }
    else
    {
        MyCom.SerialPort_Close();
    }
}

void MainWindow::SerialPort_Connect_Ok_Slots()
{
    ui->pushButton_OpenPort->setText("关闭串口");
    ui->comboBox_BaudRate->setEnabled(false);
    ui->comboBox_PortName->setEnabled(false);
    qDebug() << "串口打开成功";
}

void MainWindow::SerialPort_Disconnect_Slots()
{
    ui->pushButton_OpenPort->setText("打开串口");
    ui->comboBox_BaudRate->setEnabled(true);
    ui->comboBox_PortName->setEnabled(true);
    qDebug() << "关闭串口成功";
}
