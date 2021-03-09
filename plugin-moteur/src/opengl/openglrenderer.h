#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "renderer.h"

class OpenGLRenderer: public Renderer
{
public:
    OpenGLRenderer();
    virtual void initialize() override;
    virtual void updatePerspective(int w, int h) override;
    virtual void startRendering() override;
    virtual void endRendering() override;
    virtual void renderQuad(int x, int y, int width, int height, const float* color) override;
};

#endif // OPENGLRENDERER_H
