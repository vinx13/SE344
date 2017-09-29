/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef UIAPPLICATION_H
#define UIAPPLICATION_H

#include <GLFW/glfw3.h>

class UIApplication {
public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    UIApplication();

    void runLoop();

    void terminate();

private:
    // window size
    const unsigned int kScreenWidth = 800;
    const unsigned int kScreenHeight = 600;

    GLFWwindow *window_;

    void processInput();


};


#endif //UIAPPLICATION_H
