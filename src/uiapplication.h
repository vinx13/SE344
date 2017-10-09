/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef UIAPPLICATION_H
#define UIAPPLICATION_H

#include <memory>
#include <GLFW/glfw3.h>

#include "program.h"
#include "sphere.h"
#include "camera.h"
#include "uiobject.h"
#include "obj.h"


class UIApplication {
public:
    static UIApplication &getInstance();

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    void init(const std::string &modelPath);

    void runLoop();

    void terminate();

private:
    static UIApplication instance__;

    UIApplication();

    // window size
    const unsigned int kScreenWidth = 800;
    const unsigned int kScreenHeight = 600;
    const int kOpenGLVersionMajor = 3, kOpenGLVersionMinor = 3;

    GLFWwindow *window_;
    std::shared_ptr<Program> program_;
    std::unique_ptr<UIObject> object_, object2_;
    std::unique_ptr<Camera> camera_;
    std::unique_ptr<ObjLoader> loader_;

    void processInput();

    double lastTime_, currentTime_;

};


#endif //UIAPPLICATION_H
