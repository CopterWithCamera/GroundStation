#include "imagedatamanage.h"

imagedatamanage::imagedatamanage(QObject *parent) : QObject(parent)
{
    image_save_flag = false;

    QString file_full = QCoreApplication::applicationFilePath();    //获取路径信息
    QFileInfo fi = QFileInfo(file_full);                            //建立文件信息对象
    file_path = fi.absolutePath();                                  //提取路径信息

    image_counter = 0;  //图像计数器
}

void imagedatamanage::Image_Generate()
{
    //更新图像数组
    /*从imageTmpArray提取数据更新imageByteArray*/
    for(int i=0;i<Img_Size;i++)
    {
        imageByteArray[i*3] = imageByteArray[i*3+1] = imageByteArray[i*3+2] = imageTmpArray[i];
    }

    //生成Qimage图像
    //图像数组数据保存到DisImage图像变量
    DisImage = QImage(imageByteArray, Img_Width, Img_Height, QImage::Format_RGB888);
    //DisImage变换大小并存入imgScaled
    imgScaled = DisImage.scaled(400,240,Qt::KeepAspectRatio);

    Image_Save();
}

void imagedatamanage::Image_Save()
{
    if(image_save_flag)
    {
        //文件名序号累加，清零后为0，加1后从1开始保存
        image_counter++;

        //生成文件名
        QString strtmp;
        strtmp.setNum(image_counter);
        QString filename = file_path + "/" + strtmp + ".jpg";

        //保存图片
        QPixmap imgtmp = QPixmap::fromImage(DisImage);
        imgtmp.save(filename,"jpg",100);
    }
}

