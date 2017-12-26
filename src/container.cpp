/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include "container.h"

ContainerDrawable::ContainerDrawable(const std::shared_ptr<Program> &program) : Drawable(program) {
    float vertices[] = {
        1.f, -1.f, -1.f,
        1.f, -1.f, 1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,
        1.f, 1.f, -1.f,
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f
    };
    unsigned short indices[] = {
        4,0,3,
        3,4,7,
        2,6,7,
        2,7,3,
        1,5,2,
        5,6,2,
        0,4,1,
        4,5,1,
        0,1,2,
        0,2,3
    };
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_coords_);
    glGenBuffers(1, &ebo_);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    assert(glGetError()==0);
}

void ContainerDrawable::render(const glm::mat4 &model) {
    assert(program_.get());
    //assert(nFaces_ > 0);
    program_->use();
    program_->setMat4("model", model);
    int err = glGetError();
    assert(err==0);
    glBindVertexArray(vao_);
    assert(glGetError()==0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    assert(glGetError()==0);
    //for (int i = 0 ; i < 5; i++)
    glDrawElements(GL_LINE_LOOP, 30, GL_UNSIGNED_SHORT, 0);
    assert(glGetError()==0);

}

Container::Container(const glm::mat4 &model) : UIObject(model) {

}

