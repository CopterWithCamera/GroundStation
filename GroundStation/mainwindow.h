#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QDebug>
#include "serialport.h"
#include "imagedatamanage.h"

#include "main.h"
#include "ui_disimage.h"
#include "imagedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void StringToHex(QString str, QByteArray &senddata);    //字符串转16进制
    char ConvertHexChar(char ch);

private slots:

    void on_pushButton_GetPort_clicked();

    void on_pushButton_OpenPort_clicked();

    void on_DataDisplay_Send_2_clicked();

    void on_DataDisplay_Send_clicked();

    void Display_on_DataDisplay_ReceiveBox(QByteArray data);

    void on_Button_pathchange_clicked();

    void on_checkBox_imagesave_stateChanged(int arg1);

    void on_Button_numberclear_clicked();

    void DisplayImage();

private:
    Ui::MainWindow *ui;

    //类、线程
    QThread MyComThread;
    SerialPort MyCom;

    QThread MyImgThread;
    imagedatamanage MyImg;

};

#endif // MAINWINDOW_H
