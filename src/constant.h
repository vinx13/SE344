/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_CONSTANT_H
#define SE344_CONSTANT_H

#include <cmath>
#include <glm/glm.hpp>

constexpr double pi = 3.14159265358979323836;
constexpr int kNumParticles = 10000;
constexpr float kSmoothRadius = 1.5;
constexpr float kSmoothRadius2 = kSmoothRadius * kSmoothRadius;
constexpr float kSmoothRadius3 = kSmoothRadius2 * kSmoothRadius;
constexpr float kPoly6 =
    315.0 / 64.0 / pi / kSmoothRadius3 / kSmoothRadius3 / kSmoothRadius3;
constexpr float kSpiky = -45.0 / pi / kSmoothRadius3 / kSmoothRadius3;
constexpr float kViscosity = 0.1;
constexpr float kParticleMass = 1.0;
constexpr float kIdealGasCoeff = 1000.0;
constexpr float kStandardDensity = 1.2;
constexpr auto kGravity = glm::vec3(0.0, -150.f, 0.0);
constexpr float kLaplacianViscosity = 45.0 * pi / kSmoothRadius3 / kSmoothRadius3;

constexpr float kXMax = 18.0;
constexpr float kYMax = 18.0;
constexpr float kZMax = 18.0;
constexpr int kNX = static_cast<int>(kXMax / kSmoothRadius + 1);
constexpr int kNY = static_cast<int>(kYMax / kSmoothRadius + 1);
constexpr int kNZ = static_cast<int>(kYMax / kSmoothRadius + 1);
constexpr int kGridSize = kNX * kNY * kNZ;

constexpr float kTimeStep = 0.02f;
constexpr float kRebounceCoeff = 0.9f;


constexpr float kMcStep = 0.5f;
constexpr float kMcMargin = kSmoothRadius / kMcStep;
constexpr int kMcNx = static_cast<int>((kXMax + 2 * kMcMargin) / kMcStep + 1);
constexpr int kMcNy = static_cast<int>((kYMax + 2 * kMcMargin) / kMcStep + 1);
constexpr int kMcNz = static_cast<int>((kZMax + 2 * kMcMargin) / kMcStep + 1);
constexpr int kMcGridSize = kMcNx * kMcNy * kMcNz;

constexpr float kMcThreshold = 2.5f * kParticleMass / kPoly6;
constexpr float kMcPolyThreshold = kParticleMass * .6f / kPoly6;

#endif //SE344_CONSTANT_H
