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
#include <vector>
#include <memory>
#include <GL/gl3w.h>

class Shader {
public:
    unsigned int id;
    GLenum type;

    Shader(GLenum type, const std::string &sourceFile);
    Shader(GLenum type, const std::vector<std::string> &sourceFiles);

private:
    std::vector<std::string> readShaderFile(const std::vector<std::string> &files);

    void checkCompileErrors();
};

#endif //SHADER_H
