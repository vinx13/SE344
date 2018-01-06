/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "mesher.h"
#include "sph.h"
#include "mesh.h"
#include "marching_cube.h"

Mesher::Mesher() {
    scalarField_ = std::make_unique<ScalarField<float>>();
}

Cube::Cube(int x, int y, int z, float *prevSlice, float *slice) {
    static const int delta[][3] = {
        {0, 0, 1},
        {1, 0, 1},
        {1, 0, 0},
        {0, 0, 0},
        {0, 1, 1},
        {1, 1, 1},
        {1, 1, 0},
        {0, 1, 0}
    };

    for (int i = 0; i < 8; i++) {
        p[i] = (glm::vec3(x + delta[i][0], y + delta[i][1], z + delta[i][2])) * kMcStep + glm::vec3(-kMcMargin);
        auto *base = delta[i][0] ? slice : prevSlice;
        val[i] = base[(delta[i][1] + y) * kMcNz + z + delta[i][2]];
    }
}

std::unique_ptr<ParticleMesh> Mesher::createMesh(const ParticleSet &particles, const Grid &grid) {
    auto sliceSize = kMcNz * kMcNy;
    auto *slices = new float[2 * sliceSize];
    Edges *edges = new Edges[2 * sliceSize];
    MarchingCube mc(kMcPolyThreshold);

    Edges emptyCube;
    emptyCube.fill(-1);

    calcSlice(particles, grid, slices + sliceSize, 0);

    for (int x = 0; x < kMcNx; ++x) {
        auto currentSliceIndex = x % 2;
        auto *slice = slices + currentSliceIndex * sliceSize,
            *prevSlice = slices + ((currentSliceIndex + 1) % 2) * sliceSize; // first run,
        auto *currentEdges = edges + (x % 2) * sliceSize,
            *prevEdges = edges + ((x + 1) % 2) * sliceSize;

        calcSlice(particles, grid, slice, x + 1);


        for (int y = 0; y < kMcNy; ++y) {

            for (int z = 0; z < kMcNz; ++z) {
                Cube cube(x, y, z, prevSlice, slice);
                const auto &back = z > 0 ? currentEdges[y * kMcNz + z - 1] : emptyCube;
                const auto &bottom = y > 0 ? currentEdges[(y - 1) * kMcNz + z] : emptyCube;
                const auto &left = x > 0 ? prevEdges[y * kMcNz + z] : emptyCube;
                currentEdges[y * kMcNz + z] =
                    mc.polygonize(cube, back, bottom, left);
            }
        }

    }


    delete[] slices;
    delete[] edges;

    return mc.buildMesh();
}

void Mesher::calcSlice(const ParticleSet &particles, const Grid &grid, float *slice, int x) const {
    for (int y = 0; y <= kMcNy; ++y) {
        for (int z = 0; z <= kMcNz; ++z) {
            slice[y * kMcNz + z] = calcDensityAt(particles, grid, x, y, z);
        }
    }
}

float Mesher::calcDensityAt(const ParticleSet &particles, const Grid &grid, int x, int y, int z) const {
    float density = 0.0;
    glm::vec3 point = glm::vec3(x, y, z) * kMcStep + glm::vec3(-kMcMargin);
    int gridX = static_cast<int>(point.x / kSmoothRadius), gridY = static_cast<int>(point.y /
                                                                                    kSmoothRadius), gridZ = static_cast<int>(
        point.z / kSmoothRadius);

    for (int i = std::max(gridX - 1, 0); i <= std::min(gridX + 1, kNX - 1); i++) {
        for (int j = std::max(gridY - 1, 0); j <= std::min(gridY + 1, kNY - 1); j++) {
            for (int k = std::max(gridZ - 1, 0); k <= std::min(gridZ + 1, kNZ - 1); k++) {
                auto gridIndex = i * kNY * kNZ + j * kNZ + k;
                for (int particle = grid.starts[gridIndex];
                     density < kMcThreshold & particle < grid.ends[gridIndex]; particle++) {

                    assert(particle < kNumParticles);
                    auto r = (particles.positions[particle] - point);
                    float sqrDiff = std::max(0.f, kSmoothRadius2 - glm::dot(r, r));
                    density += particles.densities[particle] * sqrDiff * sqrDiff * sqrDiff;
                }
            }
        }
    }
    return density;
}
