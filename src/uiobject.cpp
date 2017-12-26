/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include "uiobject.h"

UIObject::UIObject(const glm::mat4 &model): model_(model) {

}

UIMovingObject::UIMovingObject(const glm::mat4 &model) : UIObject(model), speed_() {

}

void UIObject::setDrawable(std::shared_ptr<Drawable> drawable) {
    this->drawable_ = std::move(drawable);
}

void UIObject::render() {
    drawable_->render(model_);
}

void UIMovingObject::update(double deltaTime) {
    double dx = speed_.x * deltaTime;
    double a = kGravityCoff + speed_.y * 0.005;

    double dy = speed_.y * deltaTime + 0.5 * a * deltaTime * deltaTime;
    double dz = speed_.z * deltaTime;
    speed_.y += deltaTime * kGravityCoff;

    model_ = glm::translate(model_, glm::vec3(dx, dy, dz));

    // std::cout << model_[3][1] << std::endl;
    if (model_[3][1] < -5.0) {
        speed_.y = -0.9 * speed_.y;
    }
}

void UIMovingObject::bounce() {
    speed_.y = 3;
}
