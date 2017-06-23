#include "imagesave.h"

ImageSave::ImageSave(QObject *parent) : QObject(parent)
{
    image_counter = 0;  //图像计数器

    QString file_full = QCoreApplication::applicationFilePath();    //获取路径信息
    QFileInfo fi = QFileInfo(file_full);                            //建立文件信息对象
    file_path = fi.absolutePath();                                  //提取路径信息
}

void ImageSave::Image_Save()
{
    //文件名序号累加，清零后为0，加1后从1开始保存
    image_counter++;

    //生成文件名
    QString strtmp;
    strtmp.setNum(image_counter);
    QString filename = file_path + "/" + strtmp + ".bmp";

    //保存图片
    QPixmap imgtmp = QPixmap::fromImage(DisImage);
    imgtmp.save(filename,"bmp",100);
}
