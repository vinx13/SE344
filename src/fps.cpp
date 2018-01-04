/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#include "fps.h"
#include "uiapplication.h"

FpsCounter::FpsCounter() :
    position_(5, UIApplication::getInstance().getScreenHeight() - 40) {
    renderer_ = std::make_unique<TextRenderer>(FontManager::getDefault());
}

void FpsCounter::render() {
    renderer_->render(string_, position_.x, position_.y, 1.f);
}

void FpsCounter::update(double deltaTime) {
    string_ = "FPS: " + std::to_string(1.0 / deltaTime);
}
