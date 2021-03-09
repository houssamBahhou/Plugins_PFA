#include "vbooglrenderer.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

VboOGLRenderer::VboOGLRenderer() = default;

VboOGLRenderer::~VboOGLRenderer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);
}

static std::string readFile(const std::string& fileName)
{
    std::stringstream stringStream;
    std::ifstream fileStream(fileName.c_str());

    stringStream << fileStream.rdbuf();

    fileStream.close();
    return stringStream.str();
}

static GLuint loadShader(const std::string& fileName, GLenum shaderType)
{
    GLuint shaderId = glCreateShader(shaderType);
    std::string shaderCode = readFile(fileName);
    const char* shaderCodeC = shaderCode.c_str();
    glShaderSource(shaderId, 1, &shaderCodeC, nullptr);
    glCompileShader(shaderId);

    GLint success = 0;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        char infoLog[maxLength];
        glGetShaderInfoLog(shaderId, maxLength, &maxLength, infoLog);

        std::cerr << "Compilation of the shader " << fileName << " failed: " << infoLog << std::endl;

        // We don't need the shader anymore.
        glDeleteShader(shaderId);

        // Use the infoLog as you see fit.

        // In this simple program, we'll just leave
        return 0;
    }

    return shaderId;
}

static GLuint loadProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
    // Load shaders
    GLuint vertId = loadShader(vertexShader, GL_VERTEX_SHADER);
    GLuint fragId = loadShader(fragmentShader, GL_FRAGMENT_SHADER);
    // Create program
    GLuint programId = glCreateProgram();
    glAttachShader(programId, vertId);
    glAttachShader(programId, fragId);
    glLinkProgram(programId);
    // Delete shader references
    glDeleteShader(vertId);
    glDeleteShader(fragId);
    return programId;
}

void VboOGLRenderer::initialize()
{
    glewInit();
    programId = loadProgram("shader.vert", "shader.frag");
    projMatrix = glGetUniformLocation(programId, "projection");

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);
}

void VboOGLRenderer::updatePerspective(int w, int h)
{
    glm::mat4 projection = glm::ortho(0.0f, (float)w, 0.0f, (float)h, -100.0f, 100.0f);
    glUseProgram(programId);
    glUniformMatrix4fv(projMatrix, 1, GL_FALSE, glm::value_ptr(projection));
}

void VboOGLRenderer::startRendering()
{
    vertices.clear();
    indices.clear();
}

void VboOGLRenderer::endRendering()
{
    glUseProgram(programId);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    // Set VBO data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Vertex format onfiguration
    glVertexAttribPointer(0, 3, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)); // position
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, r)); // color

    // Actual draw
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
}

void VboOGLRenderer::renderQuad(int x, int y, int width, int height, const float* color)
{
    size_t first_index = vertices.size();

    vertices.emplace_back(x, y + height, 0, color[0] * 255, color[1] * 255, color[2] * 255);
    vertices.emplace_back(x,  y, 0, color[0] * 255, color[1] * 255, color[2] * 255);
    vertices.emplace_back(x + width, y, 0, color[0] * 255, color[1] * 255, color[2] * 255);
    vertices.emplace_back(x + width, y + height, 0, color[0] * 255, color[1] * 255, color[2] * 255);

    indices.push_back(first_index + 0);
    indices.push_back(first_index + 1);
    indices.push_back(first_index + 3);
    indices.push_back(first_index + 3);
    indices.push_back(first_index + 1);
    indices.push_back(first_index + 2);
}
