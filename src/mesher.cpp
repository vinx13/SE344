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

Mesher::Mesher() {
    scalarField_ = std::make_unique<std::array<float, kMcGridSize>>();
}

void Mesher::buildScalarField(const ParticleSet &particles) {
    for (auto x = 0; x < kMcNx; x++) {
        for (auto y = 0; y < kMcNy; y++) {
            for (auto z = 0; z < kMcNz; z++) {
                scalarField_->at(getMcGridIndex(x, y, z)) = calculateDensityAt(particles, x, y, z);
            }
        }
    }
}

std::vector<glm::vec3> Mesher::generateIsoSurface() {
    return std::vector<glm::vec3>();
}

std::unique_ptr<Mesh> Mesher::createMesh(const ParticleSet &particles) {
    buildScalarField(particles);
    auto vertices = generateIsoSurface();
    return std::unique_ptr<Mesh>(static_cast<Mesh*>(new TriangleMesh(std::move(vertices))));
}

int Mesher::getMcGridIndex(int x, int y, int z) {
    return x * kMcNy * kMcNz + y * kMcNz + z;
}

float Mesher::calculateDensityAt(const ParticleSet &particles, int x, int y, int z) {
    auto point = glm::vec3(x - kMcMargin, y - kMcMargin, z - kMcMargin);
    auto gridX = static_cast<int>((point.x) / kSmoothRadius);
    auto gridY = static_cast<int>((point.y) / kSmoothRadius);
    auto gridZ = static_cast<int>((point.z) / kSmoothRadius);

    float density = 0.f;
    for (int particleX = std::max(gridX - 1, 0); particleX <= std::min(gridX + 1, kNX); particleX++) {
        for (int particleY = std::max(gridY - 1, 0); particleY <= std::min(gridY + 1, kNY); particleY++) {
            for (int particleZ = std::max(gridZ - 1, 0); particleZ <= std::min(gridZ + 1, kNZ); particleZ++) {
                int particleIndex = particleX * kNY * kNZ + particleY * kNZ + particleZ;
                auto r = point - particles.positions[particleIndex];
                auto dist2 = glm::dot(r, r);
                auto sqrDiff = kSmoothRadius2 - dist2;
                if (sqrDiff <= 0) continue;
                auto deltaDensity = kPoly6 * sqrDiff * sqrDiff * sqrDiff * kParticleMass;
                density += deltaDensity;
                if (density > kMcThreshold) return density;
            }
        }
    }
    return density;
}
