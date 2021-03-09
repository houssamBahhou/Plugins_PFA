#include <QApplication>
#include <QElapsedTimer>
#include <iostream>

#ifdef USE_VULKAN

#include "vulkan/vulkanwindow.h"

#else

#include "opengl/mainwindow.h"

#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QElapsedTimer timer;

#ifdef USE_VULKAN
    VulkanWindow w;
#else
    MainWindow w;
#endif
    timer.start();
    w.show();
    std::cout << "(For OpenGL) Time to render the window " << timer.elapsed() << "ms" << std::endl;
    return QApplication::exec();
}
