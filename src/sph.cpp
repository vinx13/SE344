/*
 *
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <glm/glm.hpp>
#include <random>
#include "sph.h"
#include "mesher.h"
#include "uiapplication.h"

SphSim::SphSim(float width, float height, float depth) {


    grid_ = std::make_unique<Grid>();
    particles_ = std::make_unique<ParticleSet>();

    setReverseRotation(glm::mat3(1.f));

    createParticles();
    updateGrid();
}


void SphSim::createParticles() {
    auto gen = std::default_random_engine();
    auto distX = std::uniform_real_distribution<float>(kXMax),
        distY = std::uniform_real_distribution<float>(kYMax),
        distZ = std::uniform_real_distribution<float>(kZMax);

    for (int i = 0; i < kNumParticles; i++) {
        particles_->positions[i] = glm::vec3(distX(gen), distY(gen), distZ(gen));
        particles_->velocities[i] = glm::vec3(0.f);
    }
}

SphSim::~SphSim() {
}


inline void SphSim::updateGrid() {

    std::copy_n(grid_->start.begin(), kNX * kNY * kNZ, grid_->prevStart.begin());
    std::copy_n(grid_->end.begin(), kNX * kNY * kNZ, grid_->prevEnd.begin());
    std::copy_n(grid_->next.begin(), kNumParticles, grid_->prevNext.begin());
    grid_->clear();


    std::fill_n(grid_->starts_.begin(), kGridSize, -1);
    std::fill_n(grid_->ends_.begin(), kGridSize, -1);

    for (int i = 0; i < kNumParticles; i++) {
        auto &position = particles_->positions[i];
        int g = static_cast<int>(position.x / kSmoothRadius) * kNY * kNZ +
                static_cast<int>(position.y / kSmoothRadius) * kNZ + static_cast<int>(position.z / kSmoothRadius);
        if (grid_->starts_[g] < 0) {
            grid_->starts_[g] = i;
            grid_->ends_[g] = i;
        } else {
            grid_->nexts_[grid_->ends_[g]] = i;
            grid_->ends_[g] = i;
        }
        grid_->nexts_[i] = -1;
    }

    int c = 0;
    for (int g = 0; g < kGridSize; ++g) {
        grid_->starts[g] = c;
        for (int p = grid_->starts_[g]; p >= 0; p = grid_->nexts_[p]) {
            grid_->positions_[c] = particles_->positions[p];
            grid_->velocities_[c] = particles_->velocities[p];
            ++c;
        }
        grid_->ends[g] = c;
    }

    std::copy_n(grid_->velocities_.begin(), kNumParticles, particles_->velocities.begin());
    std::copy_n(grid_->positions_.begin(), kNumParticles, particles_->positions.begin());
}

void SphSim::updateDensities() {
    for (int i = 0; i < kNumParticles; i++) {
        auto &particlePosition = particles_->positions[i];
        forEachNeighbor(i, [&](int neighbor) {
            auto &neighborPosition = particles_->positions[neighbor];
            auto r = particlePosition - neighborPosition;
            auto dist2 = glm::dot(r, r); // dist = ||r||^2
            auto sqrDiff = kSmoothRadius2 - dist2; // h^2 - r^2
            if (sqrDiff > 0) {
                auto deltaDensity = (kParticleMass * kPoly6) * sqrDiff * sqrDiff * sqrDiff; // m_j * W_{ij}
                particles_->densities[i] += deltaDensity;
                particles_->densities[neighbor] += deltaDensity;
            }
        });
    }
}

void SphSim::updateAccelerations() {
    for (int i = 0; i < kNumParticles; i++) {
        float particleDensity = particles_->densities[i];

        auto direction = reverseRotation_*glm::normalize(particles_->positions[i] - glm::vec3(10.f));
        particles_->accelerations[i] += axis_ * (kGravity[1] + externalForce_) + direction * 0.5f*externalForce_;

        auto particlePosition = particles_->positions[i];
        float particleDensityRecip = kParticleMass / particleDensity;

        forEachNeighbor(i, [&](int neighbor) {
            auto r = particlePosition - particles_->positions[neighbor];
            auto dist2 = glm::dot(r, r); // ||r||^2
            if (dist2 <= kSmoothRadius2) {
                float dist = glm::length(r); // ||r||
                auto neighborDensity = particles_->densities[neighbor];
                auto neighborDensityRecip = kParticleMass / neighborDensity;
                auto sqrDiff = kSmoothRadius2 - dist2; // h^2 - r^2
                auto diff = kSmoothRadius - dist; // h - r

                auto accPressure = dist == 0.f ? glm::vec3(0.f) :
                                   kIdealGasCoeff * kParticleMass *
                                   (particleDensity + neighborDensity -
                                    2 * kStandardDensity) * kSpiky * diff * diff /
                                   2.f * r / dist / particleDensity / neighborDensity;

                particles_->accelerations[i] -= accPressure;
                particles_->accelerations[neighbor] += accPressure;

                auto accViscosity =
                    kViscosity * (particles_->velocities[neighbor] - particles_->velocities[i])
                    * kLaplacianViscosity * diff * kParticleMass;

                particles_->accelerations[i] += accViscosity / particleDensity;
                particles_->accelerations[neighbor] -= accViscosity / neighborDensity;

            }
        });

    }
    externalForce_ *= 0.3;
}

void SphSim::updateParticles() {
    for (int p = 0; p < kNumParticles; p++) {
        auto &velocity = particles_->velocities[p];
        auto &position = particles_->positions[p];
        velocity += kTimeStep * particles_->accelerations[p];
        position += kTimeStep * velocity;

        if (position.x < 0.f) {
            position.x = 0.f;
            velocity.x *= -kRebounceCoeff;
        } else if (position.x > kXMax) {
            position.x = kXMax;
            velocity.x *= -kRebounceCoeff;
        }

        if (position.y < 0.f) {
            position.y = 0.f;
            velocity.y *= -kRebounceCoeff;
        } else if (position.y > kYMax) {
            position.y = kYMax;
            velocity.y *= -kRebounceCoeff;
        }

        if (position.z < 0.f) {
            position.z = 0.f;
            velocity.z *= -kRebounceCoeff;
        } else if (position.z > kZMax) {
            position.z = kZMax;
            velocity.z *= -kRebounceCoeff;
        }
    }
}

void SphSim::update(double deltaTime) {
    std::fill_n(particles_->densities.begin(), kNumParticles, 0.f);
    std::fill_n(particles_->accelerations.begin(), kNumParticles, glm::vec3(0.f));

    updateGrid();
    updateDensities();
    updateAccelerations();
    updateParticles();

}

const ParticleSet &SphSim::getParticles() const {
    return *particles_;
}

const Grid &SphSim::getGrid() const {
    return *grid_;
}

void SphSim::setAxis(const glm::vec3 &axis) {
    SphSim::axis_ = axis;
}

void SphSim::setReverseRotation(const glm::mat3 &model) {
    reverseRotation_ = model;

    setAxis(glm::normalize(reverseRotation_ * glm::vec3(0.f, 1.f, 0.f)));
}

// external force in right direction
void SphSim::setExternalForce(float externalForce) {
    externalForce = std::min(externalForce, 350.f);
    std::cout << "External force " << externalForce << std::endl;
    SphSim::externalForce_ = externalForce;
}
