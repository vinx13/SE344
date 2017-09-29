/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <iostream>

#include "uiapplication.h"

UIApplication::UIApplication() {

    window_ = glfwCreateWindow(kScreenWidth, kScreenHeight, "SE344 Project", NULL, NULL);
    if (window_ == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window_);
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
