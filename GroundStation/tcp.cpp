#include "tcp.h"

tcp::tcp(QObject *parent) : QObject(parent)
{
    tcpSocket = NULL;

    connect(this,tcp::Tcp_Open_signals,this,tcp::Tcp_Open_Slots);
    connect(this,tcp::Tcp_Close_signals,this,tcp::Tcp_Close_Slots);
}

void tcp::Tcp_Open(QString tcpIp, QString tcpPort)
{
    emit Tcp_Open_signals(tcpIp,tcpPort);
}

void tcp::Tcp_Close()
{
    emit Tcp_Close_signals();
}

void tcp::Tcp_Open_Slots(QString tcpIp, QString tcpPort)
{
    //创建TCP类
    if(tcpSocket != NULL)   //把原来的删掉
    {
        tcpSocket->abort();
        tcpSocket->deleteLater();
        tcpSocket = NULL;
    }

    if(tcpIp==NULL||tcpPort==NULL)//判断IP和PORT是否为空
    {
        return;
    }

    tcpSocket = new QTcpSocket;//申请堆空间有TCP发送和接受操作

    //建立槽
    //连接成功信号
    connect(tcpSocket,SIGNAL(connected()),this,SLOT(connectUpdata()));

    //根据tcpIp连接主机
    tcpSocket->connectToHost(tcpIp,tcpPort.toInt());//连接主机
}

//连接成功
void tcp::connectUpdata()
{
    emit Tcp_Connect_ok_Signals();

    //收到信息
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(Get_From_Port()));  //TCP对象给信号用的槽

    //错误信号
    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(ErrorHandle(QAbstractSocket::SocketError)));

    //从外部接到信息
    connect(this,tcp::Get_From_Port,this,tcp::Send_To_Tcp);
}

void tcp::Tcp_Close_Slots()
{
    emit Tcp_Disconnect_Signals();

    if(tcpSocket == NULL)
        return;

    //disconnect(this,tcp::Get_From_Port,this,tcp::Send_To_Tcp);

    tcpSocket->abort();
    tcpSocket->deleteLater();   //这个删除方式更温和一些
    tcpSocket=NULL;
}

//处理错误
void tcp::ErrorHandle(QAbstractSocket::SocketError)
{
    Tcp_Close();    //关闭TCP连接
}

//从程序中发来的信息发给Tcp
void tcp::Send_To_Tcp()
{
    tcpSocket->write(In_To_Tcp_Buff);
}

//TCP接收
void tcp::Get_From_Port()
{
    QByteArray buf = tcpSocket->readAll();    //读入全部数据
    if(!buf.isEmpty())
    {
        Out_Of_Tcp_Buff = buf;
        //Data_analysis();
        emit Tcp_Out_Of_Tcp_Data_Signals(Out_Of_Tcp_Buff);
    }
}

//程序向tcp口发送数据
void tcp::Tcp_In_To_Tcp(QByteArray data)
{
    In_To_Tcp_Buff = data;
    emit Send_To_Tcp_Signals();
}

void tcp::Data_analysis()
{
    int size = Out_Of_Tcp_Buff.length();

    unsigned char tmp;

    for(int i=0;i<size;i++)
    {
        tmp = Out_Of_Tcp_Buff[i];

        Byte_Handle_Image(tmp);
        Byte_Handle_Fps(tmp);
    }
}

void tcp::Byte_Handle_Image(unsigned char data)
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
        else if(data = 0x01)    //防止包头包尾相接
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
            emit Tcp_Get_Image_Signals();    //发出图像信号
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}

void tcp::Byte_Handle_Fps(unsigned char data)
{
    static int mycase = 0;
    static int counter = 0; //记录一个包里面的数据位数
    static unsigned char a[4];

    switch(mycase)
    {
    case 0:
        if(data == 0x04)    //包头4
            mycase = 1;
        break;
    case 1:
        if(data == 0xFB)    //必须连续接入包头2
            mycase = 2;
        else if(data = 0x04)    //防止包头包尾相接
            mycase = 1;
        else
            mycase = 0;
        break;
    case 2:                 //解包
        a[counter] = data;  //数据存入数组中
        counter++;  //计数累加
        if(counter >=  4) //按照帧长度收满一帧，开始检查包尾
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
            float tmp;
            float *mp = (float*)a;

            tmp = *mp;

            emit Tcp_Get_Fps_Signals(tmp);    //发出图像信号
        }
        mycase = 0;   //接收状态都是要归零的
        break;
    default:
        break;
    }
}
