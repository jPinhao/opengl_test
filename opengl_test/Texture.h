#pragma once

#include <memory>
#include <string>

#include <SOIL.h>

class Texture
{
public:
    Texture(){}
    Texture(const std::string& file)
    {
        unsigned char* image = SOIL_load_image(file.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        //wrap properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        //interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GenerateMipmap()
    {
        glBindTexture(GL_TEXTURE_2D, textureID);
        glGenerateMipmap(textureID);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    inline GLuint ID() const { return textureID; }

private:
    GLuint textureID;
    int width;
    int height;
};