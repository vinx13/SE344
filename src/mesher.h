/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_MESHER_H
#define SE344_MESHER_H

#include <algorithm>
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
    static constexpr auto kSize = (kMcNx+1)*(kMcNy+1)*(kMcNz+1);
    T &at(int x, int y, int z) {
        return value[getMcGridIndex(x, y, z)];
    }
    const T &at(int x, int y, int z) const {
        return value[getMcGridIndex(x, y, z)];
    }
    void clear() {
        std::fill_n(value.begin(), kSize, 0.f);
    }
private:
    int getMcGridIndex(int x, int y, int z) const { return x * (kMcNy+1) * (kMcNz+1) + y * (kMcNz+1) + z; }

    std::array<T, kSize> value;
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

    void calcDensityField(const ParticleSet &particles, const Grid &grid);

    float getDensityAt(int x, int y, int z) const;
};


#endif //SE344_MESHER_H
