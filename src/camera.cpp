/*
 * 
 * Author: Wuwei Lin
 *
 * Copyright (c) 2017 Wuwei Lin. All rights reserved.
 *
 */


#include <glm/gtc/matrix_transform.hpp>

#include "camera.h"
#include <iostream>


// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front_(glm::vec3(0.0f, 0.0f, -1.0f)),
                                                                           zoom_(1) {
    position_ = position;
    worldUp_ = up;
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front_(
    glm::vec3(0.0f, 0.0f, -1.0f)), zoom_(1) {
    position_ = glm::vec3(posX, posY, posZ);
    worldUp_ = glm::vec3(upX, upY, upZ);
    yaw_ = yaw;
    pitch_ = pitch;
    updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position_, position_ + front_, up_);
}

glm::vec3 Camera::getPosition() const {
    return position_;
}

glm::mat4 Camera::getProjectionMatrix() const {
    const auto perspective = glm::perspective(glm::radians(60.0f), 1.33f, 0.1f, 100.0f);
    return perspective;
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::onKeyboard(kCameraMovement direction, float deltaTime) {
    float delta = kSpeed * deltaTime;
    switch (direction) {
        case Up:
            position_ += up_ * delta;
            break;
        case Down:
            position_ -= up_ * delta;
            break;
        case Forward:
            position_ += front_ * delta;
            break;
        case Backward:
            position_ -= front_ * delta;
            break;
        case Left:
            position_ -= right_ * delta;
            break;
        case Right:
            position_ += right_ * delta;
            break;
        default:
            break;
    }
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::onMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= kMouseSensitivity;
    yoffset *= kMouseSensitivity;

    yaw_ += xoffset;
    pitch_ += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch_ > 89.0f)
            pitch_ = 89.0f;
        if (pitch_ < -89.0f)
            pitch_ = -89.0f;
    }

    // Update front_, right_ and Up Vectors using the updated Eular angles
    updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::onMouseScroll(float yoffset) {
    if (zoom_ >= 1.0f && zoom_ <= 45.0f)
        zoom_ -= yoffset;
    if (zoom_ <= 1.0f)
        zoom_ = 1.0f;
    if (zoom_ >= 45.0f)
        zoom_ = 45.0f;
}

void Camera::updateCameraVectors() {
    // Calculate the new front_ vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front.y = sin(glm::radians(pitch_));
    front.z = sin(glm::radians(yaw_)) * cos(glm::radians(pitch_));
    front_ = glm::normalize(front);
    // Also re-calculate the right_ and Up vector
    right_ = glm::normalize(glm::cross(front_,
                                       worldUp_));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up_ = glm::normalize(glm::cross(right_, front_));
}
