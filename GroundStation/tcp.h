#ifndef TCP_H
#define TCP_H

#include <QObject>
//#include <QTcpSocket>

class tcp : public QObject
{
    Q_OBJECT
public:
    explicit tcp(QObject *parent = nullptr);

    bool Tcp_Open(QString tcpIp,QString tcpPort);
    bool Tcp_Close();

signals:

public slots:

private:
    QTcpSocket *tcpSocket;//直接建立TCP套接字类
};

#endif // TCP_H
