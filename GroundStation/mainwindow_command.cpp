#include "mainwindow.h"
#include "ui_mainwindow.h"

QByteArray MainWindow::Set_Command(unsigned char command)
{
    QByteArray a(5,0x00);
    a[0] = 0xAA;
    a[1] = 0xAA;
    a[2] = command;
    a[3] = 0x01;
    a[4] = 0x01;
    return a;
}

void MainWindow::on_Command_Image_stateChanged(int arg1)
{
    if(arg1)
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x01));
    }
    else
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x02));
    }
}

void MainWindow::on_Command_Result_stateChanged(int arg1)
{
    if(arg1)
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x03));
    }
    else
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x04));
    }
}

void MainWindow::on_Command_Wave_stateChanged(int arg1)
{
    if(arg1)
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x05));
    }
    else
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x06));
    }
}

void MainWindow::on_Command_Fps_stateChanged(int arg1)
{
    if(arg1)
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x07));
    }
    else
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x08));
    }
}

void MainWindow::on_Command_Sd_stateChanged(int arg1)
{
    if(arg1)
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x09));
    }
    else
    {
        MyCom.SerialPort_In_To_Port(Set_Command(0x0A));
    }
}
