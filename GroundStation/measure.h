#ifndef MEASURE_H
#define MEASURE_H

#include <QObject>
#include <QTimer>

#include "main.h"

class Measure : public QObject
{
    Q_OBJECT
public:
    explicit Measure(QObject *parent = nullptr);

signals:
    void Reveive_Fps_Updata_Signals(double fps);

public slots:

private slots:
    void Timer_Handler();

private:
    QTimer MyTimer;

};

#endif // MEASURE_H
