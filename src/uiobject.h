/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef UIOBJECT_H
#define UIOBJECT_H

#include <memory>
#include <glm/glm.hpp>
#include "drawable.h"
#include "speed.h"

class UIObject {
public:

    UIObject(glm::mat4 model = glm::scale(glm::mat4(1.f), glm::vec3(0.1f)));

    void setDrawable(std::shared_ptr<Drawable> drawable);

    void render();

    void update(double deltaTime);

    void bounce();

private:
    glm::mat4 model_;
    std::shared_ptr<Drawable> drawable_;
    Speed speed_;
};


#endif //UIOBJECT_H
