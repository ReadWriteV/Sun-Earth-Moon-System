#pragma once

#include <GL\glew.h>
#include <SOIL2.h>

#include <iostream>
#include <fstream>
#include <string>

namespace utils
{
    std::string readShaderSource(const std::string &file)
    {
        std::string content;
        std::ifstream fileStream(file, std::ios::in);
        if (fileStream.fail())
        {
            std::cout << "open " << file << " failed" << std::endl;
            std::exit(1);
        }
        std::string line;
        while (!fileStream.eof())
        {
            std::getline(fileStream, line);
            content.append(line + "\n");
        }
        fileStream.close();
        return content;
    }

    void printShaderLog(GLuint shader)
    {
        int len = 0;
        int chWritrn = 0;
        char *log;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            log = new char[len];
            glGetShaderInfoLog(shader, len, &chWritrn, log);
            std::cout << "Shader Info Log: " << log << std::endl;
            delete log;
        }
    }

    void printProgramLog(GLuint prog)
    {
        int len = 0;
        int chWritrn = 0;
        char *log;
        glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
        if (len > 0)
        {
            log = new char[len];
            glGetProgramInfoLog(prog, len, &chWritrn, log);
            std::cout << "Program Info Log: " << log << std::endl;
            delete log;
        }
    }

    bool checkOpenGLError()
    {
        bool foundError = false;
        auto glErr = glGetError();
        while (glErr != GL_NO_ERROR)
        {
            std::cout << "glError: " << glErr << std::endl;
            foundError = true;
            glErr = glGetError();
        }
        return foundError;
    }

    GLuint createShaderProgram()
    {
        GLint vertCompiled, fragCompiled, linked;

        auto vertShaderStr = readShaderSource("../shader/vertShader.glsl");
        auto vshaderSource = vertShaderStr.c_str();

        auto fragShaderStr = readShaderSource("../shader/fragShader.glsl");
        auto fshaderSource = fragShaderStr.c_str();

        auto vShader = glCreateShader(GL_VERTEX_SHADER);
        auto fShader = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vShader, 1, &vshaderSource, nullptr);
        glShaderSource(fShader, 1, &fshaderSource, nullptr);

        glCompileShader(vShader);
        checkOpenGLError();
        glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
        if (vertCompiled != 1)
        {
            std::cout << "vertex compilation failed" << std::endl;
            printShaderLog(vShader);
        }

        glCompileShader(fShader);
        checkOpenGLError();
        glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
        if (fragCompiled != 1)
        {
            std::cout << "fragment compilation failed" << std::endl;
            printShaderLog(fShader);
        }

        auto vfProgram = glCreateProgram();
        glAttachShader(vfProgram, vShader);
        glAttachShader(vfProgram, fShader);

        glLinkProgram(vfProgram);
        checkOpenGLError();
        glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
        if (linked != 1)
        {
            std::cout << "linking failed" << std::endl;
            printProgramLog(vfProgram);
        }

        return vfProgram;
    }

    GLuint loadTexture(const std::string &file)
    {
        GLuint textureID = SOIL_load_OGL_texture(file.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
        if (textureID == 0)
        {
            std::cout << "could not load texture file " << file << std::endl;
            std::exit(1);
        }
        return textureID;
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