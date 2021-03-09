#-------------------------------------------------
#
# Project created by QtCreator 2020-12-14T14:53:03
#
#-------------------------------------------------

QT       += core gui opengl

LIBS += -lopengl32 -L"C:/dev/cpp/libraries/GLEW/glew-2.0.0-mingw/lib/" -lglew32

INCLUDEPATH += "C:/dev/cpp/libraries/GLEW/glew-2.0.0-mingw/include" "C:/dev/cpp/libraries/GLM/"

move_shaders.path = $$DESTDIR
move_shaders.files += src/shader.frag
move_shaders.files += src/shader.vert
INSTALLS += move_shaders

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prototype-moteur
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH += ./src

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    oglwidget.cpp \
    src/openglrenderer.cpp \
    src/vbooglrenderer.cpp \
    src/vulkanrenderer.cpp \
    src/vulkanwindow.cpp

HEADERS += \
        mainwindow.h \
    oglwidget.h \
    src/openglrenderer.h \
    src/renderer.h \
    src/vbooglrenderer.h \
    src/vulkanrenderer.h \
    src/vulkanwindow.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    src/shader.frag \
    src/shader.vert

RESOURCES += prototype-moteur.qrc