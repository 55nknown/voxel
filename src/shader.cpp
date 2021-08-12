#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "shader.h"
#include "renderer.h"

Shader::Shader(const std::string &filepath) : _filepath(filepath), _program(0)
{
    ShaderProgramSource source = ParseShader();
    _program = CreateShader(source.VertexShader, source.FragmentShader);
}

Shader::~Shader() 
{
    glCall(glDeleteProgram(_program));
}

ShaderProgramSource Shader::ParseShader()
{
    std::ifstream stream(_filepath);
    if (!stream.is_open())
    {
        std::cerr << "Failed to open shader file: " << _filepath << std::endl;
        ASSERT(false);
        return {};
    }

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAMGENT = 1
    };

    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    std::string line;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAMGENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    ShaderProgramSource source;
    source.VertexShader = ss[0].str();
    source.FragmentShader = ss[1].str();

    stream.close();
    return source;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string &source)
{
    glCall(unsigned int id = glCreateShader(type));
    const char *src = source.c_str();
    glCall(glShaderSource(id, 1, &src, NULL));
    glCall(glCompileShader(id));

    int result;
    glCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        glCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char message[length];
        glCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl
                  << message << std::endl;
        glCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
    glCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glCall(glAttachShader(program, vs));
    glCall(glAttachShader(program, fs));
    glCall(glLinkProgram(program));
    glCall(glValidateProgram(program));

    glCall(glDeleteShader(vs));
    glCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    glCall(glUseProgram(_program));
}

void Shader::Unbind() const
{
    glCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    glCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string &name, const glm::mat4 &mat) 
{
    glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat[0][0]));
}

void Shader::SetUniform1i(const std::string &name, int v0)
{
    glCall(glUniform1i(GetUniformLocation(name), v0));
}

void Shader::SetUniform1f(const std::string &name, float v0) 
{
    glCall(glUniform1f(GetUniformLocation(name), v0));
}

int Shader::GetUniformLocation(const std::string &name)
{
    if (_uniforms.count(name))
        return _uniforms[name];

    glCall(int location = glGetUniformLocation(_program, name.c_str()));
    if (location == -1)
        std::cout << "Warning: Failed to get uniform location for " << name << std::endl;
    else
        _uniforms[name] = location; 
    return location;
}
