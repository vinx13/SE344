/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include <cassert>
#include<iostream>
#include <algorithm>
#include <vector>
#include<string>
#include <glm/ext.hpp>
#include "program.h"

#include "shader.h"

Program::Program() : is_linked(false), vert_(nullptr), frag_(nullptr), geom_(nullptr) {
    this->id = glCreateProgram();
}

Program::Program(const std::shared_ptr<Shader> s0, const std::shared_ptr<Shader> s1) : Program{} {
    this->attachShader(std::move(s0));
    this->attachShader(std::move(s1));
}

Program::~Program() {
    glDeleteProgram(this->id);
}

void Program::setMat4(const char *name, const glm::mat4 &mat) {
    auto loc = glGetUniformLocation(this->id, name);
    assert(glGetError() == 0);
    glUniformMatrix4fv(loc, 1, false, glm::value_ptr(mat));

    assert(glGetError() == 0);
}

void Program::setVec3(const char *name, const glm::vec3 &vec) {
    auto loc = glGetUniformLocation(this->id, name);
    assert(glGetError() == 0);
    glUniform3fv(loc, 1, glm::value_ptr(vec));

    assert(glGetError() == 0);
}

void Program::attachShader(const std::shared_ptr<Shader> s) {
    switch (s->type) {
        case GL_VERTEX_SHADER:
            this->vert_ = s;
            break;
        case GL_FRAGMENT_SHADER:
            this->frag_ = s;
            break;
        case GL_GEOMETRY_SHADER:
            this->geom_ = s;
            break;
    }
    glAttachShader(this->id, s->id);
}

void Program::link() {
    glLinkProgram(this->id);

    this->check(GL_LINK_STATUS);
    // glValidateProgram(this->id);
    //this->check(GL_VALIDATE_STATUS);
    this->is_linked = true;


    if (this->vert_) {
        glDetachShader(this->id, this->vert_->id);
        this->vert_ = nullptr;
    }
    if (this->frag_) {
        glDetachShader(this->id, this->frag_->id);
        this->frag_ = nullptr;
    }
    if (this->geom_) {
        glDetachShader(this->id, this->geom_->id);
        this->geom_ = nullptr;
    }
}

/**
checks a given status for this program.
@param what_to_check GL_LINK_STATUS GL_VALIDATE_STATUS GL_COMPILE_STATUS
*/
void Program::check(GLenum what_to_check) {
    GLint status;
    glGetProgramiv(this->id, what_to_check, &status);

    if (status != GL_TRUE) {
        GLint log_length;
        glGetProgramiv(this->id, GL_INFO_LOG_LENGTH, &log_length);
        auto log = std::make_unique<char[]>(log_length);
        glGetProgramInfoLog(this->id, log_length, NULL, log.get());

        std::cerr << "Program $" << what_to_check << "$ failed\n" << log.get();

        throw std::runtime_error(log.get());
    }
}

void Program::use() {
    glUseProgram(this->id);
    assert(glGetError() == 0);
}

void Program::stopUsing() {
    glUseProgram(static_cast<GLuint>(0));
}

GLint Program::getUniform(const char *name) {
    return glGetUniformLocation(this->id, name);
}

GLint Program::getAttribute(const char *name) {
    assert(this->is_linked && "Get attribute before linking");

    return glGetAttribLocation(this->id, name);
}

void Program::setAttribute(const char *name, GLuint id) {
    assert(is_linked && "Set attribute before linking");

    glBindAttribLocation(this->id, id, name);
}

