#include "serialport.h"

SerialPort::SerialPort(QObject *parent) : QObject(parent)
{

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

