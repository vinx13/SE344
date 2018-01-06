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
#include "mesher.h"

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

    program->use();
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
    auto vert = std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/vert.glsl");
    auto frag = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/frag.glsl");

    program->attachShader(vert);
    program->attachShader(frag);
    program->link();
    program->use();

    axis_ = glm::vec3(0.f, 1.f, 0.f);
    sim->setAxis(axis_);
    drawable_ = std::make_shared<ContainerDrawable>(program);

    renderer_ = std::make_unique<Renderer>();
    renderer_->setDrawParticles(false);
    renderer_->setCamera(UIApplication::getInstance().getCamera());
    renderer_->setModel(glm::translate(glm::mat4(1.f), glm::vec3(-kXMax/2,-kYMax/2,-kZMax/2)));
}

void Pool::render() {
    renderer_->render(sim->getParticles(), sim->getGrid());
//    auto ax = sim->getAxis();
//    ax = glm::normalize(glm::mat3(model_)*ax);
//    std::cout << ax.x << ' ' << ax.y << ' ' << ax.z << std::endl;
//    axisRenderer_.render(sim->getAxis(), renderer_->getModel());
}

void Pool::update(double deltaTime) {
    sim->update(deltaTime);
}

void Pool::toggleDrawParticle() {
    renderer_->setDrawParticles(!renderer_->isDrawParticles());
}

void Pool::rotateIfHit(float x, float y, float xoffset, float yoffset) {
    //auto offset = glm::vec3(kXMax / 2, kYMax / 2, kZMax / 2);
    //auto t = glm::translate(renderer_->getModel(), -offset);
    auto r = glm::rotate(glm::mat4(1.f), 0.01f * xoffset, glm::vec3(0.f,1.f,0.f));
    r = glm::rotate(r, 0.01f*yoffset, glm::vec3(-1.f,0.f,0.f));
    auto model = r*renderer_->getModel();
    renderer_->setModel(model);

    auto raxis = glm::mat3(glm::inverse(r));
    auto id = raxis * glm::mat3(r);
    //std::cout << raxis *glm::mat3(r) << std::endl;
    //axis_ = glm::normalize(raxis * axis_);
    axis_ = glm::normalize(glm::mat3(glm::inverse(model))*glm::vec3(0.f,1.f,0.f));
    sim->setAxis(axis_);
}
