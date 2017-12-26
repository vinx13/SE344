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
    UIObject() = default;
    UIObject(const glm::mat4 &model);

    void setDrawable(std::shared_ptr<Drawable> drawable);

    virtual void render();

    virtual ~UIObject() {}
protected:
    std::shared_ptr<Drawable> drawable_;
    glm::mat4 model_;
};

class UIMovingObject: public UIObject {
public:
    UIMovingObject(const glm::mat4 &model);

    void update(double deltaTime);

    void bounce();

private:
    Speed speed_;
};


#endif //UIOBJECT_H
