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
#include <memory>

class Mesh {
public:
    virtual void render(Program &program) = 0;
    virtual void bind() = 0;
    virtual ~Mesh() = default;
};

class ObjDrawable;
class TriangleMesh : public Mesh {
public:
    TriangleMesh (const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices);
    TriangleMesh (const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices, const std::vector<glm::vec3> &normals);

    glm::vec3 calcPlaneNormal(unsigned short i1, unsigned short i2, unsigned short i3) const;

    void calcNormals();

    const std::vector<glm::vec3> &getVertices() const;

    const std::vector<unsigned short> &getIndices() const;

    static std::shared_ptr<TriangleMesh> fromDrawable(ObjDrawable *drawable);

    void render(Program &program) override;

    void bind() override;

    void enableInstance();

    void renderInstanced(Program &program, const glm::vec3 *offsets, int n);
protected:
    GLuint vao_;
    GLuint vbo_coord_, vbo_normal_;
    GLuint ebo_;
    std::vector<glm::vec3> vertices_;
    std::vector<unsigned short> indices_;
    std::vector<glm::vec3> normals_;

    GLuint offset_buffer_;

    void generateBuffers();

    void bufferData();
};

class ParticleMesh : public TriangleMesh {
public:
    ParticleMesh (const std::vector<glm::vec3> &vertices, const std::vector<unsigned short> &indices);

    void render(Program &program) override;

    void bind() override;
};


#endif //SE344_MESH_H
