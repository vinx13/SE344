/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <iostream>
#include <GL/gl3w.h>

#include "uiapplication.h"

UIApplication::UIApplication() {

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

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

}

void UIApplication::runLoop() {
    while (!glfwWindowShouldClose(window_)) {
        // input
        processInput();

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window_);
        glfwPollEvents();
    }
}

void UIApplication::processInput() {
    if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window_, true);
}

void UIApplication::terminate() {
    glfwTerminate();
}

void UIApplication::framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
