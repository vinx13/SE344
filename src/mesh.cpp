/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "mesh.h"
#include "program.h"

ParticleMesh::ParticleMesh(const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices)
    : vertices_(vertices), indices_(indices) {
    assert(glGetError() == 0);

    calculateNormals();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_.size(), vertices_.data(), GL_STREAM_DRAW);
    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * indices.size(), indices.data(), GL_STREAM_DRAW);

    assert(glGetError() == 0);
}

void ParticleMesh::render(Program &program) {
    bind();
    program.use();
    assert(glGetError() == 0);
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_SHORT, nullptr);
    assert(glGetError() == 0);
}

void ParticleMesh::bind() {
    glBindVertexArray(vao_);
}

void ParticleMesh::calculateNormals() {

}

