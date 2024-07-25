#include "shader.hpp"

#include <string>
#include <fstream>
#include <stdexcept>

#include <glm/gtc/type_ptr.hpp>

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(shader_program);
}

void ShaderProgram::addShaderSourceFromFile(const std::filesystem::path &file, ShaderType type)
{
    if (!std::filesystem::exists(file))
    {
        throw std::runtime_error{file.string() + " not found"};
    }

    std::ifstream fileStream{file};
    if (fileStream.fail())
    {
        throw std::runtime_error{"open " + file.string() + " failed"};
    }

    std::string content;
    while (!fileStream.eof())
    {
        std::string line;
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    auto content_c_str = content.c_str();

    auto shader_id = glCreateShader(type == ShaderType::VertexShader ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);

    if (shader_id == 0)
    {
        throw std::runtime_error{"error occurs creating the shader object"};
    }

    glShaderSource(shader_id, 1, &content_c_str, nullptr);
    glCompileShader(shader_id);

    checkCompileStatus(shader_id);

    if (type == ShaderType::VertexShader)
    {
        vertex_shader = shader_id;
    }
    else
    {
        fragment_shader = shader_id;
    }
}

void ShaderProgram::link()
{
    auto program_id = glCreateProgram();
    if (program_id == 0)
    {
        throw std::runtime_error{"error occurs creating the shader program"};
    }
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);

    glLinkProgram(program_id);
    checkLinkStatus(program_id);
    shader_program = program_id;

    glDeleteShader(vertex_shader);
    vertex_shader = 0;
    glDeleteShader(fragment_shader);
    fragment_shader = 0;
}

void ShaderProgram::use() const
{
    glUseProgram(shader_program);
}

void ShaderProgram::setUniformValue(std::string_view name, const glm::mat4 &value)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderProgram::setUniformValue(std::string_view name, const glm::vec3 &value)
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void ShaderProgram::setUniformValue(std::string_view name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::checkCompileStatus(GLuint shader_id) const
{
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (success != GL_TRUE)
    {
        glGetShaderInfoLog(shader_id, static_cast<int>(std::size(infoLog)), nullptr, infoLog);
        throw std::runtime_error{"SHADER COMPILATION FAILED\n" + std::string{infoLog}};
    }
}

void ShaderProgram::checkLinkStatus(GLuint program_id) const
{
    GLint success;
    char infoLog[512];
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (success != GL_TRUE)
    {
        glGetProgramInfoLog(program_id, static_cast<int>(std::size(infoLog)), nullptr, infoLog);
        throw std::runtime_error{"PROGRAM LINK FAILED\n" + std::string{infoLog}};
    }
}

GLint ShaderProgram::getUniformLocation(std::string_view name) const
{
    assert(shader_program != 0);
    auto loc = glGetUniformLocation(shader_program, name.data());
    if (loc == -1)
    {
        throw std::runtime_error{"getUniformLocation failed " + std::string{name}};
    }
    return loc;
}
