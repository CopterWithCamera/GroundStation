#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>
#include <QImage>

#include "main.h"

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

    QStringList SerialPort_Get_Port_List();  //返回可用串口QList
    void SerialPort_Open(QString PortName, int Baud);
    void SerialPort_Close();

    //对外的收发函数
    QByteArray SerialPort_Out_Of_Port();          //供外部调用的读取数据的方法
    void SerialPort_In_To_Port(QByteArray data);           //从外部接收数据的方法

signals:
    void SerialPort_Out_Of_Port_Data_Signals(QByteArray data);  //对外直接传送数据
    void SerialPort_Get_Image_Signals();
    void SerialPort_Get_Result_Signals();
    void SerialPort_Get_Fps_Signals(double fps);

    void SerialPort_Connect_Ok_Signals();
    void SerialPort_Disconnect_Signals();

    void Send_To_Port_Signals();  //内部信号
    void SerialPort_Open_Signals(QString PortName,int Baud);
    void SerialPort_Close_Signals();

private slots:
    void Get_From_Port(); //内部从port接收数据（只作为内部转接用）

    void SerialPort_Open_Slots(QString PortName,int Baud);
    void SerialPort_Close_Slots();

private:
    void Send_To_Port();  //内部发送数据给Port的函数

    void Data_analysis();
    void Byte_Handle_Image(unsigned char data); //用于识别图像数据包
    void Byte_Handle_Result(unsigned char data);
    void Byte_Handle_Fps(unsigned char data);

    //缓存数据的Buff
    QByteArray Out_Of_Port_Buff;
    QByteArray In_To_Port_Buff;

    QSerialPort * port;         //一个Qt提供的串口对象
};

#endif // SERIALPORT_H
