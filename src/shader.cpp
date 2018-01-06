/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "shader.h"


Shader::Shader(GLenum type, const std::string &sourceFile) : Shader(type, std::vector<std::string>{sourceFile}) {

}

Shader::Shader(GLenum type, const std::vector<std::string> &sourceFiles) {
    this->id = glCreateShader(type);
    this->type = type;

    const auto &sourceCodes = readShaderFile(sourceFiles);
    std::vector<const char *>shaderSources;
    for (const auto &source: sourceCodes) {
        shaderSources.push_back(source.c_str());
    }

    glShaderSource(this->id, shaderSources.size(), shaderSources.data(), NULL);

    glCompileShader(this->id);

    checkCompileErrors();
}

std::vector<std::string> Shader::readShaderFile(const std::vector<std::string> &files) {
    std::vector<std::string> sources;
    std::ifstream shader_file;

    // ensure ifstream objects can throw exceptions
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    for (const auto &file : files) {
        try {
            std::string source_code;
            shader_file.open(file);
            std::stringstream shaderStream;
            shaderStream << shader_file.rdbuf();
            shader_file.close();
            source_code = shaderStream.str();
            sources.push_back(std::move(source_code));
        }
        catch (const std::ifstream::failure &e) {
            std::cerr << "Failed to read shader file " << file << ' ' << e.what() << std::endl;
        }
    }

    return sources;
}

void Shader::checkCompileErrors() {
    //check compiliation result
    GLint status;
    glGetShaderiv(this->id, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        GLint log_length;
        glGetShaderiv(this->id, GL_INFO_LOG_LENGTH, &log_length);

        auto log = std::make_unique<char[]>(log_length);
        glGetShaderInfoLog(this->id, log_length, NULL, log.get());

        std::cerr << "Failed to compile " << type << " shader\n" << log.get(); // FIXME: enhance error message

        glDeleteShader(this->id);

        throw std::runtime_error(log.get());
    }
}
