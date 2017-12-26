/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_CONTAINER_H
#define SE344_CONTAINER_H

#include "drawable.h"
#include "uiobject.h"

class ContainerDrawable: public Drawable {
public:
    ContainerDrawable();
    ContainerDrawable(const std::shared_ptr<Program> &program);

    void render(const glm::mat4 &model) override;

private:
    GLuint vbo_coords_, vao_, ebo_;
};

class Container: public UIObject {
public:
    Container(const glm::mat4 &model);

};

#endif //SE344_CONTAINER_H
