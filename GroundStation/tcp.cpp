#include "tcp.h"

tcp::tcp(QObject *parent) : QObject(parent)
{

}

bool tcp::Tcp_Open(QString tcpIp, QString tcpPort)
{
//    //创建TCP类
//    if(tcpSocket) delete tcpSocket;//如果有指向其他空间直接删除
//    tcpSocket = new QTcpSocket(this);//申请堆空间有TCP发送和接受操作

//    //获取IP和Port字符串并判断是否可用
//    tcpIp = ui->IPLineEdit->text();
//    tcpPort = ui->portLineEdit->text();
//    if(tcpIp==NULL||tcpPort==NULL)//判断IP和PORT是否为空
//    {
//        QMessageBox msgBox;
//        msgBox.setText("IP or PORT is Empty");
//        msgBox.exec();
//        return;
//    }

//    //根据tcpIp连接主机
//    tcpSocket->connectToHost(tcpIp,tcpPort.toInt());//连接主机

//    //建立槽

//    //连接成功信号
//    //connected()信号是在成功建立同host的连接后发出的
//    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectUpdata()));
}
