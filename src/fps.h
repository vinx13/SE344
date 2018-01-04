/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_FPS_H
#define SE344_FPS_H

#include <memory>
#include <string>
#include <glm/glm.hpp>
#include "text_render.h"

class FpsCounter {
public:
    FpsCounter();

    void render();

    void update(double deltaTime);

private:
    std::unique_ptr<TextRenderer> renderer_;
    std::string string_;
    const glm::ivec2 position_;
};


#endif //SE344_FPS_H
