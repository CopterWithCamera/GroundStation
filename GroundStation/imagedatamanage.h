#ifndef IMAGEDATAMANAGE_H
#define IMAGEDATAMANAGE_H

#include <QObject>
#include <QImage>
#include <QFileDialog>
#include <QCoreApplication>

#include "main.h"

class imagedatamanage : public QObject
{
    Q_OBJECT
public:
    explicit imagedatamanage(QObject *parent = nullptr);

    bool image_save_flag;
    QString file_path;   //文件路径
    int image_counter;  //图像计数器

signals:

public slots:
    void Image_Save();
    void Image_Generate();

};

#endif // IMAGEDATAMANAGE_H
