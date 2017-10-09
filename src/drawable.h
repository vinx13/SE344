/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include "program.h"

class Drawable {
public:
    Drawable() = default;

    Drawable(std::shared_ptr<Program> program): program_(program) { }

    void setProgram(std::shared_ptr<Program> program) { program_ = program; }

    virtual void render(const glm::mat4 &model) = 0;

    virtual ~Drawable() = default;

protected:
    std::shared_ptr<Program> program_;
};


#endif //DRAWABLE_H
