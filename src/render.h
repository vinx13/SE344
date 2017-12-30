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

class Program;

class ParticleSet;

class SphereMesh;

class Camera;

class Renderer {
public:
    Renderer();

    void render(const ParticleSet &particles);

    virtual ~Renderer() {}

private:
    std::shared_ptr<SphereMesh> mesh_;
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Program> program_;
    GLuint position_buffer_;
};


#endif //SE344_RENDERER_H
