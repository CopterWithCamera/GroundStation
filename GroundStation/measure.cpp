#include "measure.h"

Measure::Measure(QObject *parent) : QObject(parent)
{
    connect(&MyTimer,QTimer::timeout,this,Measure::Timer_Handler);
    MyTimer.start(5000);    //5s一次的Timer
}

//用timer计算本地fps
void Measure::Timer_Handler()
{
    double fps = fps_receive / 5.0;
    fps_receive = 0;

    emit Reveive_Fps_Updata_Signals(fps);
}
