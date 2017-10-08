/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "uiapplication.h"
#include "shader.h"
#include "program.h"
#include "sphere.h"

UIApplication UIApplication::instance__;

UIApplication &UIApplication::getInstance() {
    return instance__;
}

UIApplication::UIApplication() {
}

void UIApplication::init(const std::string &modelPath) {

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kOpenGLVersionMajor);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kOpenGLVersionMinor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window_ = glfwCreateWindow(kScreenWidth, kScreenHeight, "SE344 Project", NULL, NULL);

    if (window_ == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window_);

    if (gl3wInit()) {
        std::cerr << "Failed to init gl3w" << std::endl;
        exit(-1);
    }

    if (!gl3wIsSupported(kOpenGLVersionMajor, kOpenGLVersionMajor)) {
        std::cerr << "OpenGL " << kOpenGLVersionMajor << '.' << kOpenGLVersionMinor << " not supported" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);

    glBindVertexArray(0);
    auto vert = std::make_unique<Shader>(GL_VERTEX_SHADER, "shader/vert.glsl");
    auto frag = std::make_unique<Shader>(GL_FRAGMENT_SHADER, "shader/frag.glsl");
    program_ = std::make_unique<Program>(vert.get(), frag.get());
    program_->link();
    program_->use();
    //
    auto projection_ = glm::perspective(glm::radians(45.0f), 1.33f, 0.1f, 100.0f);

    program_->setMat4("projection", projection_);


    auto scale = glm::scale(glm::mat4(1.f), glm::vec3(0.1f));
    auto translate = glm::vec3(-2, 0, 0);
    this->object_ = std::make_unique<UIObject>(glm::translate(scale, translate));
    this->object2_ = std::make_unique<UIObject>(glm::translate(scale, -translate));

    this->loader_ = std::make_unique<ObjLoader>();

    object_->setDrawable(std::static_pointer_cast<Drawable>(std::make_shared<Sphere>(program_)));

    auto drawable = loader_->load(modelPath);
    drawable->setProgram(program_);
    object2_->setDrawable(drawable);

    // position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0
    this->camera_ = std::make_unique<Camera>(-1.f, 1.f, 1.f,
                                             0.f, 1.f, 0.f,
                                             -45.f, -45.f);
}

void UIApplication::runLoop() {
    lastTime_ = glfwGetTime();

    while (!glfwWindowShouldClose(window_)) {
        currentTime_ = glfwGetTime();
        auto delta = currentTime_ - lastTime_;

        // input
        processInput();

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program_->setMat4("view", camera_->genViewMatrix());

        object_->update(delta);
        object2_->update(delta);

        object_->render();
        object2_->render();

        lastTime_ = currentTime_;

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void UIApplication::processInput() {
    auto delta = currentTime_ - lastTime_;

    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    } else if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Up, delta);
    } else if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Down, delta);
    } else if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Left, delta);
    } else if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Right, delta);
    } else if (glfwGetKey(window_, GLFW_KEY_Z) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Forward, delta);
    } else if (glfwGetKey(window_, GLFW_KEY_C) == GLFW_PRESS) {
        camera_->onKeyboard(Camera::Backward, delta);
    }
}

void UIApplication::terminate() {
    glfwTerminate();
}

void UIApplication::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void UIApplication::mouse_callback(GLFWwindow *window, double xpos, double ypos) {
    static float lastX = 800 / 2.0f;
    static float lastY = 600 / 2.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    instance__.camera_->onMouseMovement(xoffset, yoffset);
}

void UIApplication::scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    instance__.camera_->onMouseScroll(yoffset);
}
