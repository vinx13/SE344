/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include <glm/glm.hpp>
#include "render.h"
#include "program.h"
#include "shader.h"
#include "sph.h"
#include "mesher.h"
#include "uiapplication.h"

Renderer::Renderer() {
    sphereMesh_ = std::make_unique<SphereMesh>(5, 5, 0.06);
    sphereMesh_->enableInstance();
    mesher_ = std::make_unique<Mesher>();
    auto vert = std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/particle.vert");
    auto frag = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/particle.frag");
    particleProgram_ = std::make_shared<Program>(vert, frag);
    particleProgram_->link();
    particleProgram_->use();

    vert = std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/water.vert");
    frag = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/water.frag");
    waterProgram_ = std::make_shared<Program>(vert, frag);
    waterProgram_->link();
    waterProgram_->use();

    glUniform1i(waterProgram_->getUniform("skybox"), 0);

    assert(glGetError() == 0);

}

void Renderer::render(const ParticleSet &particles, const Grid &grid) {

    auto mvp = camera_->getProjectionMatrix() * camera_->getViewMatrix() * model_;
    if (drawParticles_) {
        particleProgram_->use();
        particleProgram_->setMat4("mvp", mvp);

        sphereMesh_->bind();
        sphereMesh_->renderInstanced(*particleProgram_, particles.positions.data(), kNumParticles);
    } else {
        waterProgram_->use();
        waterProgram_->setMat4("mvp", mvp);
        waterProgram_->setVec3("cameraPos", camera_->getPosition());
        waterProgram_->setMat4("model", model_);

        auto mesh_ = mesher_->createMesh(particles, grid);

        //particleProgram_->use();
        //particleProgram_->setMat4("mvp", mvp);

        mesh_->bind();
        mesh_->render(*waterProgram_);
    }
}

void Renderer::setDrawParticles(bool drawParticles) {
    drawParticles_ = drawParticles;
}

bool Renderer::isDrawParticles() const {
    return drawParticles_;
}

void Renderer::setCamera(const std::shared_ptr<Camera> &camera) {
    camera_ = camera;
}

void Renderer::setModel(const glm::mat4 &model) {
    model_ = model;
}

const glm::mat4 &Renderer::getModel() const {
    return model_;
}


