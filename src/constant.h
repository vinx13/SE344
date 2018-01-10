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

constexpr double kPi = 3.14159265358979323836;
constexpr int kNumParticles = 7000;
constexpr float kSmoothRadius = 1.5;
constexpr float kSmoothRadius2 = kSmoothRadius * kSmoothRadius;
constexpr float kSmoothRadius3 = kSmoothRadius2 * kSmoothRadius;
constexpr float kPoly6 =
    315.0 / 64.0 / kPi / kSmoothRadius3 / kSmoothRadius3 / kSmoothRadius3;
constexpr float kSpiky = -45.0 / kPi / kSmoothRadius3 / kSmoothRadius3;
constexpr float kViscosity = 0.25;
constexpr float kParticleMass = 1.0;
constexpr float kIdealGasCoeff = 1000.0;
constexpr float kStandardDensity = 1.2;
constexpr auto kGravity = glm::vec3(0.0, -100.f, 0.0);
constexpr float kLaplacianViscosity = 45.0 * kPi / kSmoothRadius3 / kSmoothRadius3;

constexpr float kXMax = 24.0;
constexpr float kYMax = 24.0;
constexpr float kZMax = 24.0;
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

constexpr float kPolyThreshold = kParticleMass * .6f / kPoly6;

#endif //SE344_CONSTANT_H
