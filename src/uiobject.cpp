/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "uiobject.h"

#include <utility>

UIObject::UIObject(glm::mat4 model): model_(model), speed_() {

}

void UIObject::setDrawable(std::shared_ptr<Drawable> drawable) {
    this->drawable_ = std::move(drawable);
}

void UIObject::render() {
    drawable_->render(model_);
}

void UIObject::update(double deltaTime) {
    double dx = speed_.x * deltaTime;
    double dy = speed_.y * deltaTime + 0.5 * kGravityCoff * deltaTime * deltaTime;
    double dz = speed_.z * deltaTime;
    speed_.y += deltaTime * kGravityCoff;

    model_ = glm::translate(model_, glm::vec3(dx, dy, dz));

    // std::cout << model_[3][1] << std::endl;
    if (model_[3][1] < -1.0) {
        speed_.y = -0.8 * speed_.y;
    }
}
