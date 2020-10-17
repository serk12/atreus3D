QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/camera.cc \
    src/glwidget.cc \
    src/main.cpp \
    src/fpscount.cpp \
    src/mesh.cc \
    src/meshTypes.cpp \
    src/object.cpp \
    src/particle.cc \
    src/simulation.cpp

INCLUDEPATH += headers/ \
    /usr/local/lib/cpp/Eigen/
LIBS += -lGLEW

HEADERS += \
    headers/camera.h \
    headers/fpscount.h \
    headers/glwidget.h \
    headers/mesh.h \
    headers/meshTypes.h \
    headers/object.h \
    headers/particle.h \
    headers/simulation.h

FORMS += \
    forms/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitignore \
    shaders/sphere.frag \
    shaders/sphere.geo \
    shaders/sphere.vert \
    shaders/vanilla.frag \
    shaders/vanilla.vert
