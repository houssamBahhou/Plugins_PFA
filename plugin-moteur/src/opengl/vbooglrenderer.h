#ifndef VBOOGLRENDERER_H
#define VBOOGLRENDERER_H

#include "renderer.h"
#include <GL/glew.h>
#include <vector>

class VboOGLRenderer: public Renderer
{
public:
    VboOGLRenderer();
    virtual ~VboOGLRenderer() override;
    virtual void initialize() override;
    virtual void updatePerspective(int w, int h) override;
    virtual void startRendering() override;
    virtual void endRendering() override;
    virtual void renderQuad(int x, int y, int width, int height, const float* color) override;

private:

    struct Vertex
    {
        GLint x, y, z;
        GLubyte r, g, b;

        Vertex(GLint x, GLint y, GLint z, GLubyte r, GLubyte g, GLubyte b): x(x), y(y), z(z), r(r), g(g), b(b)
        { }
    };

    GLuint programId{};
    GLuint vbo{};
    GLuint ibo{};
    GLint projMatrix{};
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
};

#endif // VBOOGLRENDERER_H
