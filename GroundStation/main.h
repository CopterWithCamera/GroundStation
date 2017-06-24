#ifndef MAIN_H
#define MAIN_H

#include <QImage>
#include <QString>

#define Img_Width       80
#define Img_Height      48
#define Img_Size        Img_Width*Img_Height
#define Img_Buf_Size    Img_Size*3

extern unsigned char imageTmpArray[Img_Size];      //灰度数据临时BUFF
extern unsigned char imageByteArray[Img_Buf_Size]; //生成的RGB888图像
extern QImage DisImage;                            //生成的图像
extern QImage imgScaled;

extern unsigned char ResultTmpArray[Img_Size];      //灰度数据临时BUFF
extern unsigned char ResultByteArray[Img_Buf_Size]; //生成的RGB888图像
extern QImage DisResult;
extern QImage ResScaled;

extern QString file_path;

extern bool flag_datatrans;
extern bool flag_imagedisplay;
extern bool flag_resultdisplay;
extern bool flag_imagesave;

extern double fps_receive;

#endif // MAIN_H
