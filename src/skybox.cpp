/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2018 Wuwei Lin. All rights reserved.
 *
 */
#include <SOIL.h>
#include <GL/gl3w.h>
#include "shader.h"
#include "skybox.h"
#include "uiapplication.h"

Skybox::Skybox(const std::string &xpos, const std::string &xneg, const std::string &ypos, const std::string &yneg,
               const std::string &zpos, const std::string &zneg) {
    texture_ = SOIL_load_OGL_cubemap(xpos.c_str(), xneg.c_str(), ypos.c_str(), yneg.c_str(), zpos.c_str(), zneg.c_str(),
                                     SOIL_LOAD_RGB,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_MIPMAPS);
    assert(texture_ > 0);

    program_ = std::make_unique<Program>(
        std::make_shared<Shader>(GL_VERTEX_SHADER, "shader/skybox.vert"),
        std::make_shared<Shader>(GL_FRAGMENT_SHADER, "shader/skybox.frag")
    );
    program_->link();
    program_->use();
//    float vertices[] = {
//        // positions
//        -1.0f,  1.0f, -1.0f,
//        -1.0f, -1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//
//        -1.0f, -1.0f,  1.0f,
//        -1.0f, -1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f, -1.0f,
//        -1.0f,  1.0f,  1.0f,
//        -1.0f, -1.0f,  1.0f,
//
//        1.0f, -1.0f, -1.0f,
//        1.0f, -1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//
//        -1.0f, -1.0f,  1.0f,
//        -1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,
//        1.0f, -1.0f,  1.0f,
//        -1.0f, -1.0f,  1.0f,
//
//        -1.0f,  1.0f, -1.0f,
//        1.0f,  1.0f, -1.0f,
//        1.0f,  1.0f,  1.0f,
//        1.0f,  1.0f,  1.0f,
//        -1.0f,  1.0f,  1.0f,
//        -1.0f,  1.0f, -1.0f,
//
//        -1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f,  1.0f,
//        1.0f, -1.0f, -1.0f,
//        1.0f, -1.0f, -1.0f,
//        -1.0f, -1.0f,  1.0f,
//        1.0f, -1.0f,  1.0f
//    };

    const float vertices[] = {
        1.f, -1.f, -1.f,
        1.f, -1.f, 1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,
        1.f, 1.f, -1.f,
        1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, -1.f
    };
    const unsigned short indices[] = {
        4, 0, 3,
        3, 4, 7,
        2, 6, 7,
        2, 7, 3,
        1, 5, 2,
        5, 6, 2,
        0, 4, 1,
        4, 5, 1,
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7
    };
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    glUniform1i(program_->getUniform("skybox"), 0);

    assert(glGetError() == 0);
}

unsigned int Skybox::getTexture() const {
    return texture_;
}

void Skybox::render() {
    //auto view = UIApplication::getInstance().getProjectionMatrix() *
              // UIApplication::getInstance().getViewMatrix();// * glm::scale(glm::mat4(1.0), glm::vec3(50.f));
auto view=  UIApplication::getInstance().getViewMatrix();
    view=UIApplication::getInstance().getProjectionMatrix()*glm::mat4(glm::mat3(view));
    program_->use();
    program_->setMat4("view", view);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
//glDrawArrays(GL_TRIANGLES, 36,0);
    glDepthFunc(GL_LESS);
    assert(glGetError() == 0);


}
