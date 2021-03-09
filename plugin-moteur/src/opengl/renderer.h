#ifndef RENDERER_H
#define RENDERER_H


class Renderer
{
public:
    virtual ~Renderer() = default;
    virtual void initialize() = 0;
    virtual void updatePerspective(int w, int h) = 0;
    virtual void startRendering() = 0;
    virtual void endRendering() = 0;
    virtual void renderQuad(int x, int y, int width, int height, const float* color) = 0;
};

#endif // RENDERER_H
