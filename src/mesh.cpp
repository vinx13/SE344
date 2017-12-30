/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include "mesh.h"
#include "program.h"

TriangleMesh::TriangleMesh(const std::vector<glm::vec3> &&vertices):vertices_(vertices) {
    calculateNormals();

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)*vertices_.size(), vertices_.data(), GL_STREAM_DRAW);
}

void TriangleMesh::render(Program &program) {
    bind();
    program.use();
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.size() / 3));
}

void TriangleMesh::bind() {
    glBindVertexArray(vao_);
}

void TriangleMesh::calculateNormals() {
    
}

