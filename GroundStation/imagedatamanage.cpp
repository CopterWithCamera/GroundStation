#include "imagedatamanage.h"

imagedatamanage::imagedatamanage(QObject *parent) : QObject(parent)
{

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

    emit Image_Ok_Signals();    //发出图像生成完毕信号
}


