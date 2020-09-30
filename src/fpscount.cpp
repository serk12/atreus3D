#include "fpscount.h"

int FPSCount::fps = 9999;

FPSCount::FPSCount(QWidget *parent)
    : QLCDNumber(parent)
{
    setSegmentStyle(Filled);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &FPSCount::showFPS);
    timer->start(10);

    showFPS();
}

void FPSCount::setFPS(int fps) {
    FPSCount::fps = fps;
}

void FPSCount::showFPS() {
    display(this->fps);
}
