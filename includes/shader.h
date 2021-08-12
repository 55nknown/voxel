#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

struct ShaderProgramSource
{
    std::string VertexShader;
    std::string FragmentShader;
};

class Shader
{
private:
    unsigned int _program;
    const std::string &_filepath;
    // caching for uniforms
    std::unordered_map<std::string, int> _uniforms;

public:
    Shader(const std::string &filepath);
    ~Shader();


    void Bind() const;
    void Unbind() const;

    // Set uniforms
    void SetUniform1i(const std::string &name, int v0);
    void SetUniform1f(const std::string &name, float v0);
    void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &mat);

private:
    ShaderProgramSource ParseShader();
    unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    unsigned int CompileShader(unsigned int type, const std::string &source);
    int GetUniformLocation(const std::string &name);
};

#endif // SHADER_H