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
#include <soundio/soundio.h>

#include "program.h"
#include "sphere.h"
#include "camera.h"
#include "pool.h"
#include "uiobject.h"
#include "obj.h"
#include "skybox.h"
#include "fps.h"

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

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const std::shared_ptr<Camera> &getCamera() const;

    const unsigned int getScreenWidth() const;

    const unsigned int getScreenHeight() const;

    void onMouseDraging(float x, float y, float xoffset, float yoffset);

private:
    static UIApplication instance__;

    UIApplication();

    // window size
    const unsigned int kScreenWidth = 800;
    const unsigned int kScreenHeight = 600;
    const int kOpenGLVersionMajor = 3, kOpenGLVersionMinor = 3;

    GLFWwindow *window_;
    std::shared_ptr<Program> program_;
    std::unique_ptr<UIMovingObject> object_, object2_;
    std::shared_ptr<Camera> camera_;
    std::unique_ptr<ObjLoader> loader_;
    std::unique_ptr<Pool> pool_;
    std::unique_ptr<Skybox> skybox_;
    std::unique_ptr<FpsCounter> fps_;

    void processInput();

    double lastTime_, currentTime_;

    SoundIo *soundio_;
    SoundIoInStream *instream_;
    SoundIoDevice *device_;
    int sampleRate_;

    void initSoundIo();

    struct RecordContext {
        struct SoundIoRingBuffer *ring_buffer;
    } rc;

    static void read_callback(struct SoundIoInStream *instream, int frame_count_min, int frame_count_max);
    static void overflow_callback(struct SoundIoInStream *instream);

    void processAudio();
};


#endif //UIAPPLICATION_H
