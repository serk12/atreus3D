#ifndef FPSCOUNT_H
#define FPSCOUNT_H

#include <QLCDNumber>
#include <QTimer>

class FPSCount : public QLCDNumber
{
public:
    FPSCount(QWidget *parent = nullptr);
    static int fps;

    static void setFPS(int fps);

private slots:
    void showFPS();

};

#endif // FPSCOUNT_H
