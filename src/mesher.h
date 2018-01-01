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
#include "mesh.h"
#include "sph.h"

class ParticleSet;

template<class T>
class ScalarField {
public:
    T &at(int x, int y, int z) {
        return value[getMcGridIndex(x, y, z)];
    }
    const T &at(int x, int y, int z) const {
        return value[getMcGridIndex(x, y, z)];
    }
private:
    int getMcGridIndex(int x, int y, int z) const { return x * kMcNy * kMcNz + y * kMcNz + z; }

    std::array<T, kMcGridSize> value;
};

class Mesher {
public:
    Mesher();

    std::unique_ptr<ParticleMesh> createMesh(const ParticleSet &particles, const Grid &grid);

private:
    void buildScalarField(const ParticleSet &particles);

    float calculateDensityAt(const ParticleSet &particles, int x, int y, int z) const;

    std::unique_ptr<ScalarField<float>> scalarField_;



    float calcDensityAt(const ParticleSet &particles, const Grid &grid, int x, int y, int z) const;

    void calcSlice(const ParticleSet &particles, const Grid &grid, float *slice, int x) const;
};


#endif //SE344_MESHER_H
