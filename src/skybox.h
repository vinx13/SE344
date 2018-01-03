/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_SKYBOX_H
#define SE344_SKYBOX_H

#include <string>
#include <GL/gl3w.h>
#include "program.h"

class Skybox {
public:
    Skybox(const std::string &xpos, const std::string &xneg, const std::string &ypos, const std::string &yneg,
           const std::string &zpos, const std::string &zneg);
    void render();

    unsigned int getTexture() const;

private:
    unsigned int texture_;
    GLuint vao_, vbo_, ebo_;
    std::unique_ptr<Program> program_;

};


#endif //SE344_SKYBOX_H
