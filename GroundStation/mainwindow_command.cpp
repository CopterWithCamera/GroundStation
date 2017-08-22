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


