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
#include <sys/termios.h>
#include "drawable.h"
#include "speed.h"

class Sphere : public Drawable {
public:
    Sphere(std::shared_ptr<Program> program, bool wireMode);

    virtual void render(const glm::mat4 &model);

private:
    void generateSphere();

    void generateVertexIndices();

    void generateSolidVertexIndices();

    void generateWireVertexIndices();


    GLuint vbo_coords_, vao_, ebo_;
    std::vector<float> vertices_, normals_;
    std::vector<short> vertexIndices_;
    std::vector<short> sliceIndices_;
    std::vector<short> stackIndices_;

    bool wireMode_ = true;
    int nVert_;

    int nStacks_, nSlices_;
    float radius_;

};


#endif //SPHERE_H
