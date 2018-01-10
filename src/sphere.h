/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <glm/glm.hpp>
#include "drawable.h"
#include "speed.h"
#include "mesh.h"

class SphereMesh : public Mesh {
public:
    SphereMesh(int nSlices = 15, int nSlacks = 15, double radius = 1.0, bool wireMode = false);

    virtual void render(Program &program) override;

    void enableInstance();

    void renderInstanced(Program &program, const glm::vec3 *offsets, int n);

    void bind() override;

private:
    void generateSphere();

    void generateVertexIndices();

    void generateSolidVertexIndices();

    void generateWireVertexIndices();

    GLuint vbo_coords_, vao_, ebo_, vbo_normals_;
    std::vector<float> vertices_, normals_;
    std::vector<short> vertexIndices_;
    std::vector<short> sliceIndices_;
    std::vector<short> stackIndices_;


    bool wireMode_;
    int nVert_;
    int nStacks_, nSlices_;
    float radius_;
    GLuint offset_buffer_;
};

class Camera;

class Sphere {
public:
    void render();

    Sphere(const std::shared_ptr<Camera> &camera, const std::shared_ptr<Program> &program,
           const std::shared_ptr<Mesh> &mesh);

private:
    std::shared_ptr<Camera> camera_;
    std::shared_ptr<Program> program_;
    std::shared_ptr<Mesh> mesh_;
};


#endif //SPHERE_H
