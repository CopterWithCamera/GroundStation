﻿#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    //初始化串口指针
    port = NULL;

    connect(this,SerialPort_Open_Signals,this,SerialPort_Open_Slots);
    connect(this,SerialPort_Close_Signals,this,SerialPort_Close_Slots);
}

//获取串口列表
QStringList SerialPort::SerialPort_Get_Port_List()
{
    QStringList list;

    //把所有能用的端口都列出
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort tmp;
        tmp.setPort(info);

        list << tmp.portName(); //所以能用的串口的名称都加入list
    }

    return list;
}

//打开串口
void SerialPort::SerialPort_Open(QString PortName,int Baud)
{
    emit SerialPort_Open_Signals(PortName,Baud);
}

void SerialPort::SerialPort_Open_Slots(QString PortName,int Baud)
{
    //port是Qt串口类对象指针

    //检查是否已经打开
    if(port != NULL)
    {
        return;
    }

    //创建串口实例
    port = new QSerialPort;

    //设置串口名
    port->setPortName(PortName);

    //打开串口
    if(!port->open(QIODevice::ReadWrite))
    {
        delete port;
        port = NULL;
        return;
    }

    //设置波特率
    port->setBaudRate(Baud);

    //设置数据位数
    port->setDataBits(QSerialPort::Data8);

    //设置奇偶校验
    port->setParity(QSerialPort::NoParity);

    //设置停止位
    port->setStopBits(QSerialPort::OneStop);

    //设置流控制
    port->setFlowControl(QSerialPort::NoFlowControl);

    //连接槽函数（由于串口对象是临时创建的，所以槽函数要临时连接）
    connect(port, &QSerialPort::readyRead, this, &SerialPort::Get_From_Port);
    connect(this,SerialPort::Send_To_Port_Signals,this,SerialPort::Send_To_Port);

    emit SerialPort_Connect_Ok_Signals();

    return;
}

//关闭串口
void SerialPort::SerialPort_Close()
{
    emit SerialPort_Close_Signals();
}

void SerialPort::SerialPort_Close_Slots()
{
    //检查是否已经关闭
    if(!port->isOpen())
        return;

    disconnect(this,SerialPort::Send_To_Port_Signals,this,SerialPort::Send_To_Port);

    //关闭串口
    port->clear();
    port->close();
    port->deleteLater();
    port = NULL;

    emit SerialPort_Disconnect_Signals();
}

//程序向串口发送数据
void SerialPort::SerialPort_In_To_Port(QByteArray data)
{
    In_To_Port_Buff = data;
    emit Send_To_Port_Signals();
}

//程序从串口读出数据
QByteArray SerialPort::SerialPort_Out_Of_Port()
{
    return Out_Of_Port_Buff;
}

//======================== 内部函数 ==========================================================

//处理QtSerialPort类发出的信号
void SerialPort::Get_From_Port() //接收串口收到数据的信号
{
    QByteArray buf = port->readAll();    //读入全部数据
    if(!buf.isEmpty())
    {
        Out_Of_Port_Buff = buf;
        Data_analysis();
        emit SerialPort_Out_Of_Port_Data_Signals(Out_Of_Port_Buff);
    }
}

//接收内部信号，把Buff中的数据发送到串口
void SerialPort::Send_To_Port()
{
    port->write(In_To_Port_Buff);
}

void SerialPort::Data_analysis()
{
    int size = Out_Of_Port_Buff.length();

    unsigned char tmp;

    for(int i=0;i<size;i++)
    {
        tmp = Out_Of_Port_Buff[i];

        Byte_Handle_Image(tmp);
        Byte_Handle_Result(tmp);
        Byte_Handle_Fps(tmp);
        Byte_Handle_Mode(tmp);
    }
}

void SerialPort::Byte_Handle_Image(unsigned char data)
{
    static int mycase = 0;
    static int counter = 0; //记录一个包里面的数据位数

    switch(mycase)
    {
    case 0:
        if(data == 0x01)    //包头1
            mycase = 1;
        break;
    case 1:
        if(data == 0xFE)    //必须连续接入包头2
            mycase = 2;
        else if(data == 0x01)    //防止包头包尾相接
            mycase = 1;
        else
            mycase = 0;
        break;
    case 2:                 //解包
        imageTmpArray[counter] = data;  //数据存入数组中
        counter++;  //计数累加
        if(counter >= Img_Size) //按照帧长度收满一帧，开始检查包尾
        {
            counter = 0;
            mycase = 3;
        }
        break;
    case 3:
        if(data == 0xFE)    //验证包尾
            mycase = 4;
        else
            mycase = 0;
        break;
    case 4:
        if(data == 0x01)    //包尾验证通过，可以采纳数据
        {
            emit SerialPort_Get_Image_Signals();    //发出图像信号
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}

void SerialPort::Byte_Handle_Result(unsigned char data)
{
    static int mycase = 0;
    static int counter = 0; //记录一个包里面的数据位数

    switch(mycase)
    {
    case 0:
        if(data == 0x02)    //包头1
            mycase = 1;
        break;
    case 1:
        if(data == 0xFD)    //必须连续接入包头2
            mycase = 2;
        else if(data == 0x02)    //防止包头包尾相接
            mycase = 1;
        else
            mycase = 0;
        break;
    case 2:                 //解包
        ResultTmpArray[counter] = data;  //数据存入数组中
        counter++;  //计数累加
        if(counter >= Img_Size) //按照帧长度收满一帧，开始检查包尾
        {
            counter = 0;
            mycase = 3;
        }
        break;
    case 3:
        if(data == 0xFD)    //验证包尾
            mycase = 4;
        else
            mycase = 0;
        break;
    case 4:
        if(data == 0x02)    //包尾验证通过，可以采纳数据
        {
            emit SerialPort_Get_Result_Signals();    //发出图像信号
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}

float processing_fps = 0;
void SerialPort::Byte_Handle_Fps(unsigned char data)
{
    static int mycase = 0;
    static int counter = 0; //记录一个包里面的数据位数
    static unsigned char a[8];

    switch(mycase)
    {
    case 0:
        if(data == 0x04)    //包头4
            mycase = 1;
        break;
    case 1:
        if(data == 0xFB)    //必须连续接入包头2
            mycase = 2;
        else if(data == 0x04)    //防止包头包尾相接
            mycase = 1;
        else
            mycase = 0;
        break;
    case 2:                 //解包
        a[counter] = data;  //数据存入数组中
        counter++;  //计数累加
        if(counter >=  8) //按照帧长度收满一帧，开始检查包尾
        {
            counter = 0;
            mycase = 3;
        }
        break;
    case 3:
        if(data == 0xFB)    //验证包尾
            mycase = 4;
        else
            mycase = 0;
        break;
    case 4:
        if(data == 0x04)    //包尾验证通过，可以采纳数据
        {
            float tmp1,tmp2;
            float *mp1,*mp2;
            unsigned char * b = &(a[4]);

            mp1 = (float*)a;
            mp2 = (float*)b;

            tmp1 = *mp1;
            tmp2 = *mp2;

            emit SerialPort_Get_Fps_Signals(tmp1);    //发出信号
            processing_fps = tmp2;
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}

void SerialPort::Byte_Handle_Mode(unsigned char data)
{
    static int mycase = 0;
    static unsigned char a;

    switch(mycase)
    {
    case 0:
        if(data == 0x05)    //包头4
            mycase = 1;
        break;
    case 1:
        if(data == 0xFA)    //必须连续接入包头2
            mycase = 2;
        else if(data == 0x05)    //防止包头包尾相接
            mycase = 1;
        else
            mycase = 0;
        break;
    case 2:                 //解包
        a = data;  //数据存入数组中
        mycase = 3;
        break;
    case 3:
        if(data == 0xFA)    //验证包尾
            mycase = 4;
        else
            mycase = 0;
        break;
    case 4:
        if(data == 0x05)    //包尾验证通过，可以采纳数据
        {
            emit SerialPort_Get_Mode_Signals(a);    //发出信号
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}
