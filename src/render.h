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

    void render(const ParticleSet &particles, const Grid &grid);

    virtual ~Renderer() {}

    void setDrawParticles(bool drawParticles);

    bool isDrawParticles() const;

private:
    std::shared_ptr<SphereMesh> sphereMesh_;
    std::shared_ptr<Camera> camera_;
public:
    void setCamera(const std::shared_ptr<Camera> &camera);

private:
    std::shared_ptr<Program> particleProgram_, waterProgram_;
    std::unique_ptr<Mesher> mesher_;
    bool drawParticles_;
    glm::mat4 model_;
public:
    void setModel(const glm::mat4 &model_);

    const glm::mat4 &getModel() const;
};


#endif //SE344_RENDERER_H
