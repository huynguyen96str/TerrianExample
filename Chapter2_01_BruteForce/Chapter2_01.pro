QT       += core gui
QT += opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CBRUTE_FORCE.cpp \
    CTERRAIN.cpp \
    Core/GlobalMath.cpp \
    Core/Point3D.cpp \
    Opengl/Buffer.cpp \
    Opengl/Camera.cpp \
    Opengl/GlobalOpengl.cpp \
    Opengl/ShaderProgram.cpp \
    Opengl/Texture.cpp \
    View3D.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    CBRUTE_FORCE.h \
    CTERRAIN.h \
    Core/GlobalMath.h \
    Core/Point3D.h \
    Opengl/Buffer.h \
    Opengl/Camera.h \
    Opengl/GlobalOpengl.h \
    Opengl/ShaderProgram.h \
    Opengl/Texture.h \
    View3D.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
