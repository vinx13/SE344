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

class TriangleMesh : public Mesh {
public:
    TriangleMesh (const std::vector<glm::vec3> &&vertices);

    void render(Program &program) override;

    void bind() override;

private:
    std::vector<glm::vec3> vertices_;
    std::vector<glm::vec3> normals_;


    void calculateNormals();

    GLuint vao_;
    GLuint vbo_;
};

#endif //SE344_MESH_H
