#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "ShaderLogger.h"

//read file functionality
#include <fstream>
#include <streambuf>
class FileReader
{
public:
    static void Read(const char* file, std::string& output)
    {
        std::ifstream t(file);

        t.seekg(0, std::ios::end);
        output.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        output.assign(std::istreambuf_iterator<char>(t), std::istreambuf_iterator<char>());
    }
};

class GLSLShader
{
public:
    GLSLShader(){}

    GLSLShader(GLenum type, const char* shaderFile)
        : shaderObjId()
    {
        std::string shader;
        FileReader::Read(shaderFile, shader);
        const GLchar* shaderPtr = shader.c_str();

        //add custom deleter to shared_ptr to free shader object
        shaderObjId = std::make_shared<GLuint>(glCreateShader(type));
        glShaderSource(*shaderObjId, 1, &shaderPtr, NULL);
        glCompileShader(*shaderObjId);

        ShaderLog::LogErrors(CompilationError(*shaderObjId, shaderFile));
    }

    inline GLuint ID()
    {
        return *shaderObjId;
    }

protected:
    std::shared_ptr<GLuint> shaderObjId;
};

class VertexShader : public GLSLShader
{
public:
    VertexShader(){}

    VertexShader(const char* shaderFile)
        : GLSLShader(GL_VERTEX_SHADER, shaderFile)
    {}
};

class FragmentShader : public GLSLShader
{
public:
    FragmentShader() {}

    FragmentShader(const char* shaderFile)
        : GLSLShader(GL_FRAGMENT_SHADER, shaderFile)
    {}
};

#include <vector>
class ShaderProgram
{
public:
    ShaderProgram() {};
    ShaderProgram(const std::vector<GLSLShader*>& shaders)
    {
        programID = glCreateProgram();
        for (auto shader : shaders)
        {
            glAttachShader(programID, shader->ID());
        }
        glLinkProgram(programID);

        ShaderLog::LogErrors(LinkingError(programID));

        for (auto shader : shaders)
        {
            glDeleteShader(shader->ID());
        }
    };

    inline GLuint ID() const { return programID; }

private:
    GLuint programID;
};