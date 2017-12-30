/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "sphere.h"

SphereMesh::SphereMesh(int nSlices, int nStacks, double radius, bool wireMode)
    : nSlices_(nSlices), nStacks_(nStacks), radius_(radius), wireMode_(wireMode) {
    generateSphere();
    generateVertexIndices();

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_coords_);
    glGenBuffers(1, &ebo_);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices_.size() * sizeof(short), vertexIndices_.data(), GL_STATIC_DRAW);
}

void SphereMesh::enableInstance() {
    bind();
    glGenBuffers(1, &offset_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, offset_buffer_);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);
    assert(glGetError() == 0);
}

void SphereMesh::generateVertexIndices() {
    if (wireMode_)
        generateWireVertexIndices();
    else
        generateSolidVertexIndices();
}

void SphereMesh::generateWireVertexIndices() {
    int i, j, idx;

    vertexIndices_.clear();
    /* generate for each stack */
    for (i = 0, idx = 0; i < nStacks_ - 1; i++) {
        GLushort offset =
            1 + i * nSlices_;           /* start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j = 0; j < nSlices_; j++, idx++) {
            vertexIndices_.push_back(offset + j);
        }
        vertexIndices_.push_back(offset);
    }

    /* generate for each slice */
    for (i = 0, idx = 0; i < nSlices_; i++) {
        GLushort offset =
            1 + i;                  /* start at 1 (0 is top vertex), and we advance one slice as we go along */
        vertexIndices_.push_back(0);            /* vertex on top */
        for (j = 0; j < nStacks_ - 1; j++, idx++) {
            vertexIndices_.push_back(offset + j * nSlices_);
        }
        vertexIndices_.push_back(nVert_ - 1);              /* zero based index, last element in array... */
    }

}

void SphereMesh::generateSolidVertexIndices() {

    std::vector<int> stackIndex, sliceIndex;


    int slices = nSlices_, stacks = nStacks_;
    int i, j, idx;
    /* First, generate vertex index arrays for drawing with glDrawElements
       * All stacks, including top and bottom are covered with a triangle
       * strip.
       */
    GLushort *stripIdx;
    /* Create index vector */
    GLushort offset;

    vertexIndices_.clear();
    vertexIndices_.reserve((1 + nSlices_) * 2 * nStacks_);


    /* top stack */
    for (j = 0, idx = 0; j < nSlices_; j++, idx += 2) {
        vertexIndices_.push_back(j + 1);
        vertexIndices_.push_back(0);
    }
    vertexIndices_.push_back(1);                    /* repeat first slice's idx for closing off shape */
    vertexIndices_.push_back(0);


    /* middle stacks: */
    /* Strip indices are relative to first index belonging to strip, NOT relative to first vertex/normal pair in array */
    for (i = 0; i < stacks - 2; i++, idx += 2) {
        offset = 1 + i *
                     nSlices_;                    /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
        for (j = 0; j < nSlices_; j++, idx += 2) {
            vertexIndices_.push_back(offset + j + slices);
            vertexIndices_.push_back(offset + j);
        }
        vertexIndices_.push_back(offset + slices);        /* repeat first slice's idx for closing off shape */
        vertexIndices_.push_back(offset);
    }

    /* bottom stack */
    offset = 1 + (stacks - 2) *
                 slices;               /* triangle_strip indices start at 1 (0 is top vertex), and we advance one stack down as we go along */
    for (j = 0; j < slices; j++, idx += 2) {
        vertexIndices_.push_back(nVert_ - 1);          /* zero based index, last element in array (bottom vertex)... */
        vertexIndices_.push_back(offset + j);
    }
    vertexIndices_.push_back(nVert_ - 1);                 /* repeat first slice's idx for closing off shape */
    vertexIndices_.push_back(offset);
}

void SphereMesh::generateSphere() {
    int idx = 0;    /* idx into vertex/normal buffer */
    GLfloat x, y, z;

    /* Pre-computed circle */
    std::vector<GLfloat> sint1, sint2, cost1, cost2;

    /* number of unique vertices */
    if (nSlices_ == 0 || nStacks_ < 2) {
        nVert_ = 0;
        return;
    }

    nVert_ = nSlices_ * (nStacks_ - 1) + 2;

    float theta = 2 * M_PI / -nSlices_, phi = M_PI / -nStacks_;

    /* Allocate vertex and normal buffers, bail out if memory allocation fails */
    vertices_.resize(nVert_ * 3);
    normals_.resize(nVert_ * 3);


    /* top */
    vertices_[0] = 0.f;
    vertices_[1] = radius_;
    vertices_[2] = 0.f;
    normals_[0] = 0.f;
    normals_[1] = 1.f;
    normals_[2] = 0.f;
    idx = 3;

    /* each stack */
    for (int i = 1; i < nStacks_; i++) {

        for (int j = 0; j < nSlices_; j++, idx += 3) {
            float cosPhi = std::cos(phi * i), sinPhi = std::sin(phi * i);
            float cosTheta = std::cos(theta * j), sinTheta = std::sin(theta * j);
            x = cosTheta * sinPhi;
            z = sinTheta * sinPhi;
            y = cosPhi;

            vertices_[idx] = x * radius_;
            vertices_[idx + 1] = y * radius_;
            vertices_[idx + 2] = z * radius_;
            normals_[idx] = x;
            normals_[idx + 1] = y;
            normals_[idx + 2] = z;
        }
    }

    /* bottom */
    vertices_[idx] = 0.f;
    vertices_[idx + 1] = -radius_;
    vertices_[idx + 2] = 0.f;
    normals_[idx] = 0.f;
    normals_[idx + 1] = -1.f;
    normals_[idx + 2] = 0.f;

}

void SphereMesh::bind() {
    glBindVertexArray(vao_);
}

void SphereMesh::render(Program &program) {
    assert(glGetError() == 0);
    program.use();

    bind();
    if (wireMode_) {
        int numParts = nStacks_ - 1;
        int numVertIdxsPerPart = nSlices_ + 1;
        for (int i = 0; i < numParts; i++) {
            glDrawElements(GL_LINE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                           (GLvoid *) (sizeof(GLushort) * i * numVertIdxsPerPart));
        }
        int offset = numParts * numVertIdxsPerPart;
        numParts = nSlices_;
        numVertIdxsPerPart = nStacks_ + 1;
        for (int i = 0; i < numParts; i++) {
            glDrawElements(GL_LINE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                           (GLvoid *) (sizeof(GLushort) * (i * numVertIdxsPerPart + offset)));
        }
    } else {
        int numParts = nStacks_, numVertIdxsPerPart = (nSlices_ + 1) * 2;

        for (int i = 0; i < numParts; i++) {
            glDrawElements(GL_TRIANGLE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                           (GLvoid *) (sizeof(GLushort) * i * numVertIdxsPerPart));
        }
    }

    assert(glGetError() == 0);
}


void SphereMesh::renderInstanced(Program &program, const glm::vec3 *offsets, int n) {
    assert(glGetError() == 0);
    program.use();

    bind();
    glBindBuffer(GL_ARRAY_BUFFER, offset_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * n, offsets, GL_STREAM_DRAW);

    if (wireMode_) {
        int numParts = nStacks_ - 1;
        int numVertIdxsPerPart = nSlices_ + 1;

        for (int i = 0; i < numParts; i++) {
            glDrawElementsInstanced(GL_LINE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                                    (GLvoid *) (sizeof(GLushort) * i * numVertIdxsPerPart), n);
        }
        int offset = numParts * numVertIdxsPerPart;
        numParts = nSlices_;
        numVertIdxsPerPart = nStacks_ + 1;
        for (int i = 0; i < numParts; i++) {
            glDrawElementsInstanced(GL_LINE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                                    (GLvoid *) (sizeof(GLushort) * (i * numVertIdxsPerPart + offset)), n);
        }
    } else {
        int numParts = nStacks_, numVertIdxsPerPart = (nSlices_ + 1) * 2;

        for (int i = 0; i < numParts; i++) {
            glDrawElementsInstanced(GL_TRIANGLE_STRIP, numVertIdxsPerPart, GL_UNSIGNED_SHORT,
                                    (GLvoid *) (sizeof(GLushort) * i * numVertIdxsPerPart), n);
        }
    }

    assert(glGetError() == 0);

}

Sphere::Sphere(const std::shared_ptr<Camera> &camera, const std::shared_ptr<Program> &program,
               const std::shared_ptr<Mesh> &mesh) : camera_(camera), program_(program), mesh_(mesh) {

}
