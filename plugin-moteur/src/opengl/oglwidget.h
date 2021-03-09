#ifndef OGLWIDGET_H
#define OGLWIDGET_H

#include <GL/glew.h>
#include <QWidget>
#include <QOpenGLWidget>
#include <QRandomGenerator>
#include <QTime>
#include <memory>
#include "renderer.h"

class OGLWidget : public QOpenGLWidget
{
public:
    OGLWidget(QWidget *parent = nullptr);
    ~OGLWidget();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QRandomGenerator rng;
    QTime timer;
    std::unique_ptr<Renderer> renderer;

    const int quadWidth = 4;
    const int quadHeight = 8;
    const float colors[8][3] = {{1, 1, 1}, {1, 1, 0}, {1, 0, 1}, {0, 1, 1}, {1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};
    void renderQuad(int x, int y, int color);
};

#endif // OGLWIDGET_H
