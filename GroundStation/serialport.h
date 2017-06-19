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
    bool SerialPort_Open(QString PortName, int Baud);
    bool SerialPort_Close();

public slots:
    void SerialPort_In_To_Port();   //从其他类读入
private slots:
    void SerialPort_Get_From_Port(); //从串口读入

private:
    QSerialPort * port;         //一个Qt提供的串口对象
};

#endif // SERIALPORT_H
