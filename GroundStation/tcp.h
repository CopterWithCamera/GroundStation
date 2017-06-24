#ifndef TCP_H
#define TCP_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMessageBox>

#include "main.h"

class tcp : public QObject
{
    Q_OBJECT
public:
    explicit tcp(QObject *parent = nullptr);

    void Tcp_Open(QString tcpIp,QString tcpPort);
    void Tcp_Close();

    QByteArray Tcp_Out_Of_Tcp();          //供外部调用的读取数据的方法
    void Tcp_In_To_Tcp(QByteArray data);



signals:
    void Tcp_Connect_ok_Signals();
    void Tcp_Disconnect_Signals();

    void Tcp_Out_Of_Tcp_Data_Signals(QByteArray data); //TCP端口接到的数据向程序发送
    void Tcp_Get_Image_Signals();
    void Tcp_Get_Result_Signals();
    void Tcp_Get_Fps_Signals(float fps);

    void Send_To_Tcp_Signals();  //内部信号

    void Tcp_Open_signals(QString tcpIp,QString tcpPort);   //内部使用的Tcp打开信号
    void Tcp_Close_signals();

public slots:
    void Tcp_Open_Slots(QString tcpIp,QString tcpPort);
    void Tcp_Close_Slots();

private slots:
    void connectUpdata();   //连接成功处理
    void ErrorHandle(QAbstractSocket::SocketError);

    void Get_From_Port(); //内部从tcp接收数据（只作为内部转接用）

private:
    void Send_To_Tcp();

    void Data_analysis();
    void Byte_Handle_Image(unsigned char data);
    void Byte_Handle_Result(unsigned char data);
    void Byte_Handle_Fps(unsigned char data);

    QTcpSocket *tcpSocket;//直接建立TCP套接字类

    //缓存数据的Buff
    QByteArray Out_Of_Tcp_Buff;
    QByteArray In_To_Tcp_Buff;
};

#endif // TCP_H
