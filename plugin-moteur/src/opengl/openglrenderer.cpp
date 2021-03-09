#include "openglrenderer.h"

#include <GL/glew.h>

OpenGLRenderer::OpenGLRenderer() = default;

void OpenGLRenderer::initialize()
{

}

void OpenGLRenderer::updatePerspective(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -100, 100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void OpenGLRenderer::startRendering()
{
    glBegin(GL_QUADS);
}

void OpenGLRenderer::endRendering()
{
    glEnd();
}

void OpenGLRenderer::renderQuad(int x, int y, int width, int height, const float* color)
{
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(x, y + height, 0);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(x,  y, 0);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(x + width, y, 0);
    glColor3f(color[0], color[1], color[2]);
    glVertex3f(x + width, y + height, 0);
}
