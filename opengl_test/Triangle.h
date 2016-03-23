#pragma once

#include <GL\glew.h>

#include <vector>

#include "GLSLshader.h"

struct Triangle
{
    static constexpr GLsizeiptr numVertices()
    {
        return sizeof(vertices);
    }
    static constexpr GLsizeiptr numIndices()
    {
        return sizeof(indices);
    }

    /*GLfloat vertices[9] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };*/

    GLfloat vertices[12] = {
        0.5f,  0.5f, 0.0f,  // Top Right
        0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f   // Top Left 
    };
    GLuint indices[6] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };

    GLuint VAO;
    ShaderProgram shader;

    Triangle()
    {
        static VertexShader vtxShader("VertexShader.glsl");
        static FragmentShader fragShader("FragmentShader.glsl");
        static ShaderProgram prog(std::vector<GLSLShader*>{ &vtxShader , &fragShader });

        shader = prog;

        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
            // 0. Copy our vertices array in a buffer for OpenGL to use
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //copy the vertices into the vbo
            glBufferData(GL_ARRAY_BUFFER, numVertices(), vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices(), indices, GL_STATIC_DRAW);

            // 1. Then set the vertex attributes pointers
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);

            // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~Triangle()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

private:
    GLuint VBO;
    GLuint EBO;
};