#pragma once

#include <vector>

#include "Texture.h"
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

    GLfloat vertices[32] = {
        // Positions          // Colors           // Texture Coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // Top Right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // Top Left 
    };
    GLuint indices[6] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // First Triangle
    };

    GLuint VAO;
    ShaderProgram shader;
    Texture texture1;
    Texture texture2;

    Triangle()
    {
        static VertexShader vtxShader("VertexShader.glsl");
        static FragmentShader fragShader("FragmentShader.glsl");
        static ShaderProgram prog(std::vector<GLSLShader*>{ &vtxShader , &fragShader });
        static Texture tex1("C:\\Users\\Z\\Desktop\\untitled5.png");
        static Texture tex2("res/awesomeface.png");

        texture1 = tex1;
        texture1.GenerateMipmap();

        texture2 = tex2;
        texture2.GenerateMipmap();

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
            //positions
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
            glEnableVertexAttribArray(0);
            //color
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            //texture
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);


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