/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include <algorithm>
#include <array>
#include "mesh.h"
#include "program.h"
#include "obj.h"
#include "constant.h"

glm::vec3 TriangleMesh::calcPlaneNormal(unsigned short i1, unsigned short i2, unsigned short i3) const {
    auto &p1 = vertices_[i1], &p2 = vertices_[i2], &p3 = vertices_[i3];
    auto v1 = p2 - p1;
    auto v2 = p3 - p1;
    auto normal = glm::cross(v1, v2);
    auto normalized = glm::normalize(normal);
    return normalized;
}

void TriangleMesh::calcNormals() {
    normals_.clear();
    normals_.resize(vertices_.size(), glm::vec3(0.f));

    for (int i = 0; i < indices_.size(); i += 3) {
        auto i1 = indices_[i], i2 = indices_[i + 1], i3 = indices_[i + 2];
        const auto normal = calcPlaneNormal(i1, i2, i3);

        normals_[i1] += normal;
        normals_[i2] += normal;
        normals_[i3] += normal;
    }

    std::transform(normals_.begin(), normals_.end(), normals_.begin(),
                   [](const glm::vec3 &n) {
                       auto normal = glm::normalize(n);
                       return normal;
                   }
    );
}

TriangleMesh::TriangleMesh(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices)
    : vertices_(vertices), indices_(indices) {
    generateBuffers();

    bufferData();

    assert(glGetError() == 0);
}


TriangleMesh::TriangleMesh(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices,
                           const std::vector<glm::vec3> &normals) : vertices_(vertices), indices_(indices),
                                                                    normals_(normals) {
    generateBuffers();
    bufferData();
    assert(glGetError() == 0);
}

void TriangleMesh::bufferData() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coord_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_.size(), vertices_.data(), GL_STREAM_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normal_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals_.size(), normals_.data(), GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices_.size(), indices_.data(), GL_STREAM_DRAW);
}

void TriangleMesh::generateBuffers() {
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_coord_);
    glGenBuffers(1, &vbo_normal_);
    glGenBuffers(1, &ebo_);
}

const std::vector<glm::vec3> &TriangleMesh::getVertices() const {
    return vertices_;
}

const std::vector<unsigned short> &TriangleMesh::getIndices() const {
    return indices_;
}

std::shared_ptr<TriangleMesh> TriangleMesh::fromDrawable(ObjDrawable *drawable) {
    return std::make_shared<TriangleMesh>(drawable->getVertices(), drawable->getIndices(), drawable->getNormals());
}

void TriangleMesh::render(Program &program) {
    bind();
    program.use();
    assert(glGetError() == 0);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);
    assert(glGetError() == 0);
}

void TriangleMesh::bind() {
    glBindVertexArray(vao_);
}

void TriangleMesh::enableInstance() {
    bind();
    glGenBuffers(1, &offset_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, offset_buffer_);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 0);
    glVertexAttribDivisor(2, 1);
    assert(glGetError() == 0);
}

void TriangleMesh::renderInstanced(Program &program, const glm::vec3 *offsets, int n) {
    assert(glGetError() == 0);
    program.use();

    bind();
    glBindBuffer(GL_ARRAY_BUFFER, offset_buffer_);

    std::array<glm::vec3, kNumParticles/3> offsets_;
    for (int i = 0; i < n; i+= 3) {
        offsets_[i/3]=offsets[i];
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * n/3, offsets_.data(), GL_STREAM_DRAW);
    glDrawElementsInstanced(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, 0, n/3);

    assert(glGetError() == 0);
}

ParticleMesh::ParticleMesh(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices)
    : TriangleMesh(vertices, indices) {

    calcNormals();
    bufferData();

    assert(glGetError() == 0);
}

void ParticleMesh::render(Program &program) {
    bind();
    program.use();
    assert(glGetError() == 0);
    //glPolygonMode(GL_FRONT_AND_BACK ,GL_LINE   );
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);
    assert(glGetError() == 0);
}

void ParticleMesh::bind() {
    glBindVertexArray(vao_);
}

