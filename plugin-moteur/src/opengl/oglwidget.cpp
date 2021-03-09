#include "oglwidget.h"

#include <iostream>
#include <QLabel>
#include "vbooglrenderer.h"

OGLWidget::OGLWidget(QWidget *parent)
    : QOpenGLWidget(parent), timer()
{
    renderer = std::make_unique<VboOGLRenderer>();
}

OGLWidget::~OGLWidget() = default;

void OGLWidget::initializeGL()
{
    renderer->initialize();
    glClearColor(0,0,0,1);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
}

void OGLWidget::paintGL()
{

    rng.seed(0);
    glClear(GL_COLOR_BUFFER_BIT);

    int x = 0;
    int y = 0;
    renderer->startRendering();
    for (int j = 0; j < 100; j++)
    {
        x = 0;
        for (int i = 0; i < 400; i++)
        {
            renderer->renderQuad(x, y, quadWidth, quadHeight, colors[rng.bounded(8)]);
            x += quadWidth / 2;
        }
        y += quadHeight;
    }
    renderer->endRendering();

}

void OGLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);

    renderer->updatePerspective(w, h);
}
