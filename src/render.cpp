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

    assert(glGetError() == 0);

}

void Renderer::render(const ParticleSet &particles) {
    particleProgram_->use();
    particleProgram_->setMat4("mvp", UIApplication::getInstance().getProjectionMatrix() *
                             UIApplication::getInstance().getViewMatrix());
    if (drawParticles_) {
        sphereMesh_->bind();
        sphereMesh_->renderInstanced(*particleProgram_, particles.positions.data(), kNumParticles);
    } else {
        auto mesh_ = mesher_->createMesh(particles);
        mesh_->bind();
        mesh_->render(*particleProgram_);
    }
}

void Renderer::setDrawParticles(bool drawParticles) {
    drawParticles_ = drawParticles;
}


