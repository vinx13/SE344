/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_POOL_H
#define SE344_POOL_H

#include <memory>
#include "drawable.h"
#include "uiobject.h"
#include "render.h"
#include "sph.h"

class Particle;


class ContainerDrawable: public Drawable {
public:
    ContainerDrawable();
    ContainerDrawable(const std::shared_ptr<Program> &program);

    void render(const glm::mat4 &model) override;

private:
    GLuint vbo_coords_, vao_, ebo_;
};

class Pool: public UIObject {
public:
    Pool(const glm::mat4 &model);

    void render() override;


    std::unique_ptr<Renderer> renderer_;

    void update(double deltaTime);

    const std::vector<std::shared_ptr<Particle>> &getParticles() const;
    SphSim *sim;

    void toggleDrawMode();

    void rotateIfHit(float x, float y, float xoffset, float yoffset);

    void setBoundaryModel(const TriangleMesh &mesh, float scale = 1.f, const glm::vec3 &delta = glm::vec3(0.f));

    void onAudioInput(float left, float right);

    void setCustomMesh(const std::shared_ptr<TriangleMesh> &mesh);
private:
    std::vector<std::shared_ptr<Particle>> particles_;
    glm::vec3 axis_;
   // std::unique_ptr<SphSim> sph_;
    void scanLine(std::array<bool, kGridSize> &boundary, const glm::vec3 &p1, const glm::vec3 &p2);

    void addBoundaryPoint(std::array<bool, kGridSize> &boundary, const glm::vec3 &p);

};

#endif //SE344_POOL_H
