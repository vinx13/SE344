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

Program::Program() : is_linked(false), vert(nullptr), frag(nullptr), geom(nullptr) {
    this->id = glCreateProgram();
}

Program::Program(Shader *s0, Shader *s1) : Program{} {
    this->attachShader(s0);
    this->attachShader(s1);
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

void Program::attachShader(Shader *s) {
    switch (s->type) {
        case GL_VERTEX_SHADER:
            this->vert = s;
            break;
        case GL_FRAGMENT_SHADER:
            this->frag = s;
            break;
        case GL_GEOMETRY_SHADER:
            this->geom = s;
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


    if (this->vert) {
        glDetachShader(this->id, this->vert->id);
    }
    if (this->frag) {
        glDetachShader(this->id, this->frag->id);
    }
    if (this->geom) {
        glDetachShader(this->id, this->geom->id);
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

