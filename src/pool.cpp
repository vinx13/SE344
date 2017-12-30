/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include "pool.h"
#include "camera.h"
#include "uiapplication.h"
#include "program.h"
#include "render.h"
#include "sph.h"

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
        4, 0, 3,
        3, 4, 7,
        2, 6, 7,
        2, 7, 3,
        1, 5, 2,
        5, 6, 2,
        0, 4, 1,
        4, 5, 1,
        0, 1, 2,
        0, 2, 3
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
    assert(glGetError() == 0);

    program->setMat4("projection", UIApplication::getInstance().getProjectionMatrix());
}

void ContainerDrawable::render(const glm::mat4 &mvp) {
    assert(program_.get());
    program_->use();
    program_->setMat4("mvp", mvp);

    glBindVertexArray(vao_);
    assert(glGetError() == 0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glEnableVertexAttribArray(0);
    assert(glGetError() == 0);
    glDrawElements(GL_LINE_LOOP, 30, GL_UNSIGNED_SHORT, 0);
    assert(glGetError() == 0);
}

Pool::Pool(const glm::mat4 &model) : UIObject(model) {
    sim = new SphSim(kNX, kNY, kNZ);
    auto program = std::make_shared<Program>();
    auto vert = std::make_unique<Shader>(GL_VERTEX_SHADER, "shader/vert.glsl");
    auto frag = std::make_unique<Shader>(GL_FRAGMENT_SHADER, "shader/frag.glsl");

    program->attachShader(vert.get());
    program->attachShader(frag.get());
    program->link();
    program->use();
    drawable_ = std::make_shared<ContainerDrawable>(program);

    renderer_ = std::make_unique<Renderer>();
}

void Pool::render() {
    renderer_->render(sim->getParticles());
}

void Pool::update(double deltaTime) {
    sim->update(deltaTime);
}
