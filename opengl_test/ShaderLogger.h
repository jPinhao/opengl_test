#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

class ShaderError
{
public:
    ShaderError(GLuint objectId)
        : id(objectId)
    {}
    virtual std::string Type() const = 0;
    virtual std::string GetError() const = 0;

    const GLuint id;
};

class CompilationError : public ShaderError
{
public:
    CompilationError(GLuint shaderId, std::string fileName)
        : ShaderError(shaderId),
        file(std::move(fileName))
    {
    }

    virtual std::string Type() const override
    {
        return "SHADER::COMPILATION - " + file;
    }

    virtual std::string GetError() const override
    {
        GLint success;

        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLchar errString[512];
            glGetShaderInfoLog(id, 512, nullptr, errString);
            return errString;
        }
        else
        {
            return "Success";
        }
    }

private:
    std::string file;
};

class LinkingError : public ShaderError
{
public:
    LinkingError(GLuint programID)
        : ShaderError(programID)
    {};

    virtual std::string Type() const override
    {
        return "SHADER::LINKING";
    }

    virtual std::string GetError() const override
    {
        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            GLchar errString[512];
            glGetProgramInfoLog(id, 512, NULL, errString);
            return errString;
        }
        else
        {
            return "Success";
        }
    }
};

class ShaderLog
{
public:
#ifdef _DEBUG
    static void LogErrors(const ShaderError& error)
    {
        std::cout << error.Type() << std::endl << error.GetError() << std::endl;
    }
#else
    static void LogErrors(ShaderError& errorType) { return; }
#endif
};