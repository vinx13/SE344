/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */
#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Camera
{
public:
    const float kMouseSensitivity = 0.1;
    const float kSpeed = 0.1;

    enum kCameraMovement { Forward, Backward, Left, Right, Up, Down, ZoomIn, ZoomOut };
    // Camera Attributes
    glm::vec3 position_;
    glm::vec3 front_;
    glm::vec3 up_;
    glm::vec3 right_;
    glm::vec3 worldUp_;
    // Eular Angles
    float yaw_;
    float pitch_;
    // Camera options


    float zoom_;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0);

    // Constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 genViewMatrix();

    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void onKeyboard(kCameraMovement direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void onMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void onMouseScroll(float yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();
};

#endif //CAMERA_H
