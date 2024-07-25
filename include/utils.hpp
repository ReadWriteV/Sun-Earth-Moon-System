#pragma once

#include <string_view>

#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace utils
{
    GLuint loadTexture(std::string_view file)
    {
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(file.data(), &width, &height,
                                        &nrChannels, 0);

        if (!data)
        {
            throw std::runtime_error{"Failed to load texture: " + std::string{file}};
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
        return texture;
    }

    float *goldAmbient()
    {
        static float a[4] = {0.2473f, 0.1995f, 0.0745f, 1.0f};
        return a;
    }
    float *goldDiffuse()
    {
        static float a[4] = {0.7516f, 0.6065f, 0.2265f, 1.0f};
        return a;
    }
    float *goldSpecular()
    {
        static float a[4] = {0.6283f, 0.5559f, 0.3661f, 1.0f};
        return a;
    }
    float goldShininess()
    {
        return 51.2f;
    }
}