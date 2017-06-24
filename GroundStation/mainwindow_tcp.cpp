#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::on_Button_Tcpconnnect_clicked()
{
    if(ui->Button_Tcpconnnect->text() == "连接")
    {
        QString IP = ui->LineEdit_IP->text();
        QString Port = ui->LineEdit_port->text();
        MyTcp.Tcp_Open(IP,Port);
    }
    else if(ui->Button_Tcpconnnect->text() == "断开连接")
    {
        MyTcp.Tcp_Close();
    }
}

void MainWindow::Tcp_Connect_Ok_Slots()
{
    ui->Button_Tcpconnnect->setText("断开连接");
}

void MainWindow::Tcp_Disconnect_Slots()
{
    ui->Button_Tcpconnnect->setText("连接");
}
