/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "uiapplication.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();

    glfwSetErrorCallback(error_callback);

    UIApplication app;
    app.runLoop();
    app.terminate();

    return 0;
}

