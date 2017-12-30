/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_MESHER_H
#define SE344_MESHER_H

#include <array>
#include <vector>
#include <memory>
#include "constant.h"

class Mesh;
class ParticleSet;

class Mesher {
public:
    Mesher();

     std::unique_ptr<Mesh> createMesh(const ParticleSet &particles);

private:
    void buildScalarField(const ParticleSet &particles);
    std::vector<glm::vec3> generateIsoSurface();
    int getMcGridIndex(int x, int y, int z);

    float calculateDensityAt(const ParticleSet &particles, int x, int y, int z);
    std::unique_ptr<std::array<float, kMcGridSize>> scalarField_;
};


#endif //SE344_MESHER_H
