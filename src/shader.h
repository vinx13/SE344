/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/gl3w.h>

class Shader {
public:
    unsigned int id;
    GLenum type;

    Shader(GLenum type, const std::string &source);

private:
    std::string readShaderFile(const std::string &source);

    void checkCompileErrors();
};

#endif //SHADER_H
