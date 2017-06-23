#ifndef IMAGESAVE_H
#define IMAGESAVE_H

#include <QObject>

#include <QFileDialog>
#include <QCoreApplication>

#include "main.h"

class ImageSave : public QObject
{
    Q_OBJECT
public:
    explicit ImageSave(QObject *parent = nullptr);
    int image_counter;  //图像计数器

signals:

public slots:
    void Image_Save();
};

#endif // IMAGESAVE_H
