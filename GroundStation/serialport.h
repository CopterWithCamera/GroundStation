#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QStringList>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = nullptr);

   QStringList SerialPort_Get_Port_List();  //返回可用串口QList

private:
    QSerialPort port();         //一个Qt提供的串口对象
};

#endif // SERIALPORT_H
