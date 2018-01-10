/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#include "fps.h"
#include "text_render.h"
#include "uiapplication.h"

FpsCounter::FpsCounter() :
    position_(5, UIApplication::getInstance().getScreenHeight() - 40) {
#ifdef USE_FREETYPE
    auto fontManager = std::make_shared<FontManager>("resource/Go-Mono.ttf", 0, 24);
    renderer_ = std::make_unique<TextRenderer>(std::move(fontManager));
#endif
}

void FpsCounter::render() {
#ifdef USE_FREETYPE
    renderer_->render(string_, position_.x, position_.y, 1.f);
#endif
}

void FpsCounter::update(double deltaTime) {
    string_ = "FPS: " + std::to_string(1.0 / deltaTime);
}
