#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTimer>
#include <QDebug>

#include "main.h"
#include "serialport.h"
#include "tcp.h"
#include "imagedatamanage.h"
#include "imagesave.h"
#include "measure.h"

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

    void on_DataDisplay_Send_clicked();

    void Display_on_DataDisplay_ReceiveBox(QByteArray data);

    void on_Button_pathchange_clicked();

    void on_checkBox_imagesave_stateChanged(int arg1);

    void on_Button_numberclear_clicked();

    void DisplayImage();

    void DisplayResult();

    void on_DataDisplay_Clear_clicked();

    void Plane_fps_Dis(double fps);

    void Plane_mode_Dis(unsigned char mode);

//    void on_Button_Tcpconnnect_clicked();

//    void Tcp_Connect_Ok_Slots();

//    void Tcp_Disconnect_Slots();

    void SerialPort_Connect_Ok_Slots();
    
    void SerialPort_Disconnect_Slots();

    void Receive_fps_Dis(double fps);

    void on_checkBox_image_stateChanged(int arg1);

    void on_checkBox_result_stateChanged(int arg1);

    void on_checkBox_datatrans_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    QByteArray Set_Command(unsigned char command);

    //类、线程
    QThread MyComThread;
    SerialPort MyCom;

//    QThread MyTcpThread;
//    tcp MyTcp;

    QThread MyImgThread;
    imagedatamanage MyImg;

    QThread MyImgSaveThread;
    ImageSave MyImgSave;

    QThread MyMeasureThread;
    Measure MyMeasure;

};

#endif // MAINWINDOW_H
