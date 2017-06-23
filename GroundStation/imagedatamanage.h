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

signals:
    void Image_Ok_Signals();
    void DisImage_Ok_Signals();
public slots:
    void Image_Generate();

};

#endif // IMAGEDATAMANAGE_H
