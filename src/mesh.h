/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef SE344_MESH_H
#define SE344_MESH_H

class Program;

#include <glm/glm.hpp>
#include <vector>
#include <GL/gl3w.h>

class Mesh {
public:
    virtual void render(Program &program) = 0;
    virtual void bind() = 0;
};

class ParticleMesh : public Mesh {
public:
    ParticleMesh (const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices);

    void render(Program &program) override;

    void bind() override;

private:
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;
    std::vector<glm::vec3> normals_;


    void calculateNormals();

    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
};

#endif //SE344_MESH_H
