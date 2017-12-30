/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_RENDERER_H
#define SE344_RENDERER_H

#include <GL/gl3w.h>
#include <memory>
#include <glm/glm.hpp>
#include <vector>

#include "mesher.h"

class Program;

class ParticleSet;

class SphereMesh;

class Camera;

class Renderer {
public:
    Renderer();

    void render(const ParticleSet &particles);

    virtual ~Renderer() {}

    void setDrawParticles(bool drawParticles);

private:
    std::shared_ptr<SphereMesh> sphereMesh_;
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Program> particleProgram_;
    std::unique_ptr<Mesher> mesher_;
    bool drawParticles_;
};


#endif //SE344_RENDERER_H
