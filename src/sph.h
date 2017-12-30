/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_SPH_H
#define SE344_SPH_H

#include <vector>
#include <memory>
#include <array>
#include <algorithm>
#include "constant.h"
#include "mesh.h"
#include "program.h"
#include "shader.h"


struct Grid {
    std::array<int, kGridSize> start, end, prevStart, prevEnd;
    std::array<int, kNumParticles> next, prevNext;

    std::array<int, kGridSize> starts;
    std::array<int, kGridSize> ends;
    std::array<int, kGridSize> starts_;
    std::array<int, kGridSize> ends_;
    std::array<int, kNumParticles> nexts_;
    std::array<glm::vec3, kNumParticles> positions_;
    std::array<glm::vec3, kNumParticles> velocities_;

    Grid() { clear(); }

    void clear() {
        std::fill(start.begin(), start.end(), -1);
        std::fill(end.begin(), end.end(), -1);
        std::fill(next.begin(), next.end(), -1);
    }
};

struct ParticleSet {
    std::array<float, kNumParticles> densities;
    std::array<glm::vec3, kNumParticles> positions;
    std::array<glm::vec3, kNumParticles> velocities;
    std::array<glm::vec3, kNumParticles> accelerations;
};

class SphereMesh;
class Camera;

class SphSim {
public:

    SphSim(float width, float height, float depth);

    ~SphSim();

    void update(double deltaTime);

    const ParticleSet &getParticles() const;

private:
    std::unique_ptr<ParticleSet> particles_;
    std::unique_ptr<Grid> grid_;



    GLuint offset_buffer;

    void updateGrid();

    void updateDensities();

    void updateAccelerations();

    void updateParticles();

    void createParticles();

    template<class F>
    void forEachNeighbor(int particle, F f) {
        const auto &index = particles_->positions[particle] / kSmoothRadius;
        for (int x = std::max(0, static_cast<int>(index.x) - 1);
             x <= std::min(kNX - 1, static_cast<int>(index.x) + 1); x++) {

            for (int y = std::max(0, static_cast<int>(index.y) - 1);
                 y <= std::min(kNY - 1, static_cast<int>(index.y) + 1); y++) {

                for (int z = std::max(0, static_cast<int>(index.z) - 1);
                     z <= std::min(kNZ - 1, static_cast<int>(index.z) + 1); z++) {

                    for (int neighbor = grid_->starts[x * kNY * kNZ + y * kNZ + z];
                         neighbor < grid_->ends[x * kNY * kNZ + y * kNY + z]; neighbor++) {

                        if (particle <= neighbor) {
                            f(neighbor);
                        }
                    }
                }
            }
        }
    }

};

#endif //SE344_SPH_H
