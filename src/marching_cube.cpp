/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#include "marching_cube.h"

glm::vec3 MarchingCube::calcIntersect(const Cube &grid, int edge) {
    switch (edge) {
        case 0:
            return interpolate(grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
        case 1:
            return interpolate(grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
        case 2:
            return interpolate(grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
        case 3:
            return interpolate(grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
        case 4:
            return interpolate(grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
        case 5:
            return interpolate(grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
        case 6:
            return interpolate(grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
        case 7:
            return interpolate(grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
        case 8:
            return interpolate(grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
        case 9:
            return interpolate(grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
        case 10:
            return interpolate(grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
        case 11:
            return interpolate(grid.p[3], grid.p[7], grid.val[3], grid.val[7]);
        default:
            assert(0);
            return glm::vec3();
    }
};

Edges MarchingCube::polygonize(const Cube &grid, const Edges &back, const Edges &bottom, const Edges &left) {

    unsigned int tableIndex = 0;
    if (grid.val[0] < isolevel_) tableIndex |= 1;
    if (grid.val[1] < isolevel_) tableIndex |= 2;
    if (grid.val[2] < isolevel_) tableIndex |= 4;
    if (grid.val[3] < isolevel_) tableIndex |= 8;
    if (grid.val[4] < isolevel_) tableIndex |= 16;
    if (grid.val[5] < isolevel_) tableIndex |= 32;
    if (grid.val[6] < isolevel_) tableIndex |= 64;
    if (grid.val[7] < isolevel_) tableIndex |= 128;
    Edges vertexIndices;
    vertexIndices.fill(-1);

    int edge = kEdgeTable[tableIndex];
    if (edge == 0)
        return vertexIndices;


    auto addVertex = [&](const Cube &grid, int edge) {
        vertexIndices[edge] = vertices_.size();
        vertices_.push_back(calcIntersect(grid, edge));
    };

    if (edge & 1) {
        if (bottom[4] != -1) vertexIndices[0] = bottom[4];
        else addVertex(grid, 0);
    }
    if (edge & 2) {
        if (bottom[5] != -1) vertexIndices[1] = bottom[5];
        else addVertex(grid, 1);
    }
    if (edge & 4) {
        if (bottom[6] != -1) vertexIndices[2] = bottom[6];
        else if (back[0] != -1) vertexIndices[2] = back[0];
        else addVertex(grid, 2);
    }
    if (edge & 8) {
        if (bottom[7] != -1) vertexIndices[3] = bottom[7];
        else if (left[1] != -1) vertexIndices[3] = left[1];
        else addVertex(grid, 3);
    }
    if (edge & 16) {
        addVertex(grid, 4);
    }
    if (edge & 32) {
        addVertex(grid, 5);
    }
    if (edge & 64) {
        if (back[4] != -1) vertexIndices[6] = back[4];
        else addVertex(grid, 6);
    }
    if (edge & 128) {
        if (left[5] != -1) vertexIndices[7] = left[5];
        else addVertex(grid, 7);
    }
    if (edge & 256) {
        if (left[9] != -1) vertexIndices[8] = left[9];
        else addVertex(grid, 8);
    }
    if (edge & 512) {
        addVertex(grid, 9);
    }
    if (edge & 1024) {
        if (back[9] != -1) vertexIndices[10] = back[9];
        else addVertex(grid, 10);
    }
    if (edge & 2048) {
        if (left[10] != -1) vertexIndices[11] = left[10];
        else if (back[8] != -1) vertexIndices[11] = back[8];
        else addVertex(grid, 11);
    }

    for (int i = 0; kTriTable[tableIndex][i] != -1; i += 3) {
        indices_.push_back(vertexIndices[kTriTable[tableIndex][i]]);
        indices_.push_back(vertexIndices[kTriTable[tableIndex][i + 1]]);
        indices_.push_back(vertexIndices[kTriTable[tableIndex][i + 2]]);
    }
    return vertexIndices;
}

glm::vec3 MarchingCube::interpolate(const glm::vec3 &p1, const glm::vec3 &p2, float val1, float val2) {
    float t = (isolevel_ - val1) / (val2 - val1);
    return p1 + t * (p2 - p1);
}

std::unique_ptr<ParticleMesh> MarchingCube::buildMesh() {
    return std::make_unique<ParticleMesh>(vertices_, indices_);
}
