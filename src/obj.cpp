/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#include <fstream>
#include <sstream>
#include <iterator>

#include "obj.h"

void ObjDrawable::render(const glm::mat4 &model) {
    assert(program_.get());
    //assert(nFaces_ > 0);
    program_->use();
    program_->setMat4("model", model);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);

    glDrawElements(GL_TRIANGLES, 3 * nFaces, GL_UNSIGNED_SHORT, (GLvoid *) (0));
}

void ObjDrawable::init() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_coords_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_coords_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices_.size() * sizeof(unsigned short), vertexIndices_.data(),
                 GL_STATIC_DRAW);

}

std::shared_ptr<Drawable> ObjLoader::load(const std::string &path) {
    auto drawable = std::make_shared<ObjDrawable>();
    std::ifstream src(path);
    std::string line;
    std::string tag;

    drawable->nFaces = 0;

    while (std::getline(src, line)) {
        std::istringstream is(line);

        is >> tag;
        if (tag == "v") {
            glm::vec3 vector;
            is >> vector.x >> vector.y >> vector.z;
            drawable->vertices_.push_back(vector);
        } else if (tag == "vt") {
            glm::vec2 vector;
            is >> vector.x >> vector.y;
            drawable->textures_.push_back(vector);
        } else if (tag == "vn") {
            glm::vec3 vector;
            is >> vector.x >> vector.y >> vector.z;
            drawable->normals_.push_back(vector);
        } else if (tag == "f") {
            if (is.peek() == EOF) continue;

            int vi, vti, vni;

            int nVertexes = 0;

            std::string faceVertex;

            for (int i = 0; i < 3; i++) {
                // assume model to be triangle mesh

                is >> faceVertex;
                std::istringstream faceStream(faceVertex);

                nVertexes++;
                faceStream >> vi;
                drawable->vertexIndices_.push_back(vi - 1);
                if (faceStream.peek() != '/') continue;
                faceStream.get();

                if (faceStream.peek() != '/') { // have the second component
                    faceStream >> vti;
                    drawable->uvIndices_.push_back(vti - 1);
                }
                if (faceStream.peek() != '/') continue;
                faceStream.get();

                faceStream >> vni;
                drawable->normalIndices_.push_back(vni - 1);
            }
            drawable->nFaces++;
        }

    }

    drawable->init();

    return std::static_pointer_cast<Drawable>(drawable);
}

