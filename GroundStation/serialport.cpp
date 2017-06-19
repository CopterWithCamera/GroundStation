#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{
    //初始化串口指针
    port = NULL;
}

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

bool SerialPort::SerialPort_Open(QString PortName,int Baud)
{
    //port是Qt串口类对象指针

    //检查是否已经打开
    if(port != NULL)
    {
        return false;
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
        return false;
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
    connect(port, &QSerialPort::readyRead, this, &SerialPort::SerialPort_Get_From_Port);

    return true;
}

bool SerialPort::SerialPort_Close()
{
    //检查是否已经关闭
    if(!port->isOpen())
        return false;

    //关闭串口
    port->clear();
    port->close();
    port->deleteLater();
    port = NULL;

    return true;
}

void SerialPort::SerialPort_Get_From_Port()
{

}

void SerialPort::SerialPort_In_To_Port()
{

}


