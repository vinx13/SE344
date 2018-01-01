/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_MARCHING_CUBE_H
#define SE344_MARCHING_CUBE_H


#include <array>
#include <vector>
#include <unordered_map>
#include "mesh.h"
#include "mesher.h"
#include "constant.h"
#include "marching_cube_table.h"

typedef std::array<unsigned short, 12> Edges;

struct Cube {
    glm::vec3 p[8];
    float val[8];

    Cube(int, int, int, float *prevSlice, float *slice);
};


class MarchingCube {
public:
    MarchingCube(float isolevel) : isolevel_(isolevel) {}

    Edges polygonize(const Cube &grid, const Edges &back, const Edges &bottom, const Edges &left);

    std::unique_ptr<ParticleMesh> buildMesh();

    glm::vec3 interpolate(const glm::vec3 &p1, const glm::vec3 &p2, float val1, float val2);

private:
    float isolevel_;
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;

    glm::vec3 calcIntersect(const Cube &grid, int edge);
};


#endif //SE344_MARCHING_CUBE_H
