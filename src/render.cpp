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
#include "uiapplication.h"

Renderer::Renderer() {
    mesh_ = std::make_unique<SphereMesh>(5, 5, 0.06);
    mesh_->enableInstance();
    auto vert = std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/particle.vert");
    auto frag = std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/particle.frag");
    program_ = std::make_shared<Program>(vert, frag);
    program_->link();
    program_->use();

    assert(glGetError() == 0);

}

void Renderer::render(const ParticleSet &particles) {
    mesh_->bind();
    program_->use();
    program_->setMat4("mvp", UIApplication::getInstance().getProjectionMatrix() *
                             UIApplication::getInstance().getViewMatrix());
    mesh_->renderInstanced(*program_, particles.positions.data(), kNumParticles);
}



