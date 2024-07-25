#pragma once

#include <cassert>
#include <filesystem>

#include <glad/glad.h>
#include <glm/glm.hpp>

class ShaderProgram
{
public:
    enum class ShaderType
    {
        VertexShader,
        FragmentShader
    };

    ~ShaderProgram();

    void addShaderSourceFromFile(const std::filesystem::path &file, ShaderType type);
    void link();
    void use() const;

    void setUniformValue(std::string_view name, const glm::mat4 &value);
    void setUniformValue(std::string_view name, const glm::vec3 &value);
    void setUniformValue(std::string_view name, int value);

private:
    void checkCompileStatus(GLuint shader_id) const;
    void checkLinkStatus(GLuint shader_id) const;

    GLint getUniformLocation(std::string_view name) const;

private:
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLuint shader_program;
};