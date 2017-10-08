/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef PROGRAM_H
#define PROGRAM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"

class Program {
public:
    GLuint id;
    GLint pos_id, mvpm_id;

    Program();

    Program(Shader *s0, Shader *s1);

    ~Program();

    void attachShader(Shader *s);

    void link();

    void use();

    void stopUsing();

    GLint getUniform(const char *name);

    GLint getAttribute(const char *name);

    void setAttribute(const char *name, GLuint id);

    void setMat4(const char *name, const glm::mat4 &mat);

private:
    bool is_linked;
    Shader *vert, *frag, *geom;

    void check(GLenum what_to_check);

    // Include GLM extensions
    glm::mat4 projection_, view_;
};


#endif //PROGRAM_H
