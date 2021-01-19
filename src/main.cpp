#include "simulation.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    int r = GENERAL_SCENARY_TYPE + 1;
    while (r > 0) {
        Simulation::scenaryType = static_cast<Simulation::ScenaryType>(r - 1);
        QApplication a(argc, argv);
        Simulation w;
        w.showMaximized();
        r = a.exec();
    }
    return r;
}
