/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "shader.h"


Shader::Shader(GLenum type, const std::string &source) {
    this->id = glCreateShader(type);
    this->type = type;

    const std::string &sourceCode = readShaderFile(source);
    const char *shaderSource = sourceCode.c_str();

    glShaderSource(this->id, 1, &shaderSource, NULL);

    glCompileShader(this->id);

    checkCompileErrors();
}

std::string Shader::readShaderFile(const std::string &source) {
    std::string source_code;
    std::ifstream shader_file;

    // ensure ifstream objects can throw exceptions
    shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        shader_file.open(source);
        std::stringstream shaderStream;
        shaderStream << shader_file.rdbuf();
        shader_file.close();
        source_code = shaderStream.str();
    }
    catch (std::ifstream::failure e) {
        std::cerr << "Failed to read shader file " << source << std::endl;
    }

    return source_code;
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
