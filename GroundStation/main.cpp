#include "mainwindow.h"
#include <QApplication>

//公用图像数据
unsigned char imageTmpArray[Img_Size];      //灰度数据临时BUFF
unsigned char imageByteArray[Img_Buf_Size]; //生成的RGB888图像
QImage DisImage;                            //生成的图像
QImage imgScaled;                           //缩放后的图像

unsigned char ResultTmpArray[Img_Size];      //灰度数据临时BUFF
unsigned char ResultByteArray[Img_Buf_Size]; //生成的RGB888图像
QImage DisResult;
QImage ResScaled;


QString file_path;                          //文件路径

bool flag_datatrans = 0;
bool flag_imagedisplay = 0;
bool flag_resultdisplay = 0;
bool flag_imagesave = 0;

double fps_receive = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
