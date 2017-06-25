#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//***************** 把串口托管给线程 ************************************

    MyCom.moveToThread(&MyComThread);
    MyComThread.start();

    //更新串口列表
    ui->comboBox_PortName->addItems(MyCom.SerialPort_Get_Port_List());

    //串口对外信号连接
    connect(&MyCom,SerialPort::SerialPort_Out_Of_Port_Data_Signals,this,MainWindow::Display_on_DataDisplay_ReceiveBox);

    connect(&MyCom,SerialPort::SerialPort_Connect_Ok_Signals,this,MainWindow::SerialPort_Connect_Ok_Slots);
    connect(&MyCom,SerialPort::SerialPort_Disconnect_Signals,this,MainWindow::SerialPort_Disconnect_Slots);

//***************** 把Tcp端口托管给线程 *********************************
    MyTcp.moveToThread(&MyTcpThread);
    MyTcpThread.start();

    connect(&MyTcp,tcp::Tcp_Connect_ok_Signals,this,MainWindow::Tcp_Connect_Ok_Slots);
    connect(&MyTcp,tcp::Tcp_Disconnect_Signals,this,MainWindow::Tcp_Disconnect_Slots);

    connect(&MyTcp,tcp::Tcp_Out_Of_Tcp_Data_Signals,this,MainWindow::Display_on_DataDisplay_ReceiveBox);

//***************** 把图像处理函数托管给线程 ******************************

    MyImg.moveToThread(&MyImgThread);
    MyImgThread.start();

    //对外信号连接
    connect(&MyCom,SerialPort::SerialPort_Get_Image_Signals,&MyImg,imagedatamanage::Image_Generate);
    connect(&MyCom,SerialPort::SerialPort_Get_Result_Signals,&MyImg,imagedatamanage::Result_Generate);
    connect(&MyTcp,tcp::Tcp_Get_Image_Signals,&MyImg,imagedatamanage::Image_Generate);
    connect(&MyTcp,tcp::Tcp_Get_Result_Signals,&MyImg,imagedatamanage::Result_Generate);

    //初始化图像数组并显示图像
    for(int i=0;i<Img_Size;i++)
    {
        imageTmpArray[i] = 0xFF;
    }
    MyImg.Image_Generate();
    DisplayImage();

    for(int i=0;i<Img_Size;i++)
    {
        ResultTmpArray[i] = 0xFF;
    }
    MyImg.Result_Generate();
    DisplayResult();


    //默认不开启图像显示
    connect(&MyImg,imagedatamanage::Image_Ok_Signals,this,MainWindow::DisplayImage);
    connect(&MyImg,imagedatamanage::Result_Ok_Signals,this,MainWindow::DisplayResult);

//***************** 把图像存储托管给线程 ******************************

    //显示默认文件路径
    ui->lineEdit_filepath->setText(file_path);

    MyImgSave.moveToThread(&MyImgSaveThread);
    MyImgSaveThread.start();

    connect(&MyImg,imagedatamanage::Image_Ok_Signals,&MyImgSave,ImageSave::Image_Save);

//***************** 把一些计算任务托管给线程 ******************************

    MyMeasure.moveToThread(&MyMeasureThread);
    MyMeasureThread.start();

    connect(&MyMeasure,Measure::Reveive_Fps_Updata_Signals,this,MainWindow::Receive_fps_Dis);

//***************** 一些其余的操作 **************************************

    connect(&MyCom,SerialPort::SerialPort_Get_Fps_Signals,this,MainWindow::Plane_fps_Dis);
    connect(&MyCom,SerialPort::SerialPort_Get_Mode_Signals,this,MainWindow::Plane_mode_Dis);
    connect(&MyTcp,tcp::Tcp_Get_Fps_Signals,this,MainWindow::Plane_fps_Dis);

}

MainWindow::~MainWindow()
{
    MyMeasureThread.quit();
    MyMeasureThread.wait();

    MyComThread.quit();   //结束串口线程
    MyComThread.wait();   //等待线程完全结束

    MyTcpThread.quit();
    MyTcpThread.wait();

    MyImgThread.quit();
    MyImgThread.wait();

    MyImgSaveThread.quit();
    MyImgSaveThread.wait();

    delete ui;
}

void MainWindow::Plane_mode_Dis(unsigned char mode)
{
    int a = mode;
    QString str;
    str.setNum(a,10);
    ui->lineEdit_mode->setText(str);
}

//显示飞机端回传的fps
void MainWindow::Plane_fps_Dis(double fps)
{
    QString str;
    str.setNum(fps,10,2);
    ui->lineEdit_planefps->setText(str);
}

//显示本地接收到的fps
void MainWindow::Receive_fps_Dis(double fps)
{
    QString str;
    str.setNum(fps,10,2);
    ui->lineEdit_receivefps->setText(str);
}

//字符串转16进制（处理字符串部分）
void MainWindow::StringToHex(QString str, QByteArray &senddata) //字符串转换为十六进制数据0-F
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    senddata.resize(len/2);
    char lstr,hstr;

    for(int i=0; i<len; )
    {
        hstr=str[i].toLatin1();

        //忽略空格
        if(hstr == ' ')
        {
            i++;
            continue;
        }

        //长度计数
        i++;
        if(i >= len)
            break;

        lstr = str[i].toLatin1();
        hexdata = ConvertHexChar(hstr);
        lowhexdata = ConvertHexChar(lstr);

        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;

        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    senddata.resize(hexdatalen);
}

//字符串转16进制（ASCII码转换部分）
char MainWindow::ConvertHexChar(char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch-0x30;
    else if((ch >= 'A') && (ch <= 'F'))
        return ch-'A'+10;
    else if((ch >= 'a') && (ch <= 'f'))
        return ch-'a'+10;
    else return ch-ch;//不在0-f范围内的会发送成0
}

void MainWindow::on_DataDisplay_Clear_clicked()
{
    ui->DataDisplay_ReceiveBox->clear();
}

void MainWindow::on_DataDisplay_Send_clicked()
{
    QString str = ui->DataDisplay_SendBox->text();//从LineEdit得到字符串

    //删除空格
    str.remove(QChar(' '), Qt::CaseSensitive);

    QByteArray tmp = str.toLatin1();

    //将str转换为16进制数，存入data
    int len = tmp.length();
    if(len%2 == 1)   //如果发送的数据个数为奇数的，则在前面最后落单的字符前添加一个字符0
    {
        tmp = tmp.insert(len-1,'0'); //insert(int position, const QString & str)
    }
    QByteArray data;
    StringToHex(tmp,data);//将str字符串转换为16进制的形式

    MyCom.SerialPort_In_To_Port(data);
    MyTcp.Tcp_In_To_Tcp(data);
}

void MainWindow::Display_on_DataDisplay_ReceiveBox(QByteArray data)
{
    if(flag_datatrans)
    {
        int size = data.size();

        QString tmpstr = ui->DataDisplay_ReceiveBox->toPlainText();

        if(tmpstr.length() > 100000)
            tmpstr.clear();

        for(int i=0;i<size;i++)
        {
            unsigned char tmp = data[i];
            //QString str = QString::number(tmp,16).toUpper();
            QString str = QString("%1").arg(tmp&0xFF,2,16,QLatin1Char('0'));    //带自动补0
            tmpstr += str;
            tmpstr += " ";
        }

        ui->DataDisplay_ReceiveBox->clear();
        ui->DataDisplay_ReceiveBox->append(tmpstr);
    }
}

//将数组中的图像显示在屏幕上
void MainWindow::DisplayImage()
{
    fps_receive++;  //fps累加

    if(flag_imagedisplay)
        ui->label_image->setPixmap(QPixmap::fromImage(imgScaled)); //显示变换大小后的QImage对象
}

void MainWindow::DisplayResult()
{
    if(flag_resultdisplay)
        ui->label_result->setPixmap(QPixmap::fromImage(ResScaled)); //显示变换大小后的QImage对象
}

void MainWindow::on_Button_pathchange_clicked()
{
    QString file_path_tmp = QFileDialog::getExistingDirectory(this,"请选择数据保存文件夹","./");
    if(file_path_tmp.isEmpty())
    {
        return;
    }
    else
    {
        file_path = file_path_tmp;
        ui->lineEdit_filepath->setText(file_path);
    }
}

void MainWindow::on_Button_numberclear_clicked()
{
    MyImgSave.image_counter = 0;
}

void MainWindow::on_checkBox_imagesave_stateChanged(int arg1)
{
    flag_imagesave = (bool)arg1;
}

void MainWindow::on_checkBox_image_stateChanged(int arg1)
{
    flag_imagedisplay = (bool)arg1;
}

void MainWindow::on_checkBox_result_stateChanged(int arg1)
{
    flag_resultdisplay = (bool)arg1;
}

void MainWindow::on_checkBox_datatrans_stateChanged(int arg1)
{
    flag_datatrans = (bool)arg1;
}

