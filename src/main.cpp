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
#include "mesher.h"
#include "uiapplication.h"

static void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}


int main(int argc, char **argv) {
    glfwInit();

    std::string modelPath = argc == 2 ? argv[1] : "resource/bunny.obj";

    glfwSetErrorCallback(error_callback);

    auto &app = UIApplication::getInstance();

    app.init(modelPath);
    app.runLoop();
    app.terminate();

    return 0;
}

