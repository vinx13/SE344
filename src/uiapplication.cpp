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
#include <algorithm>

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
#if AUDIO_ENABLED
    initSoundIo();
#endif

    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);

    glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);
    glfwSetCursorPosCallback(window_, mouse_callback);
    glfwSetScrollCallback(window_, scroll_callback);
    glfwSetMouseButtonCallback(window_, mouse_button_callback);

    glBindVertexArray(0);
    auto vert = std::make_unique<Shader>(GL_VERTEX_SHADER, "shader/vert.glsl");
    auto frag = std::make_unique<Shader>(GL_FRAGMENT_SHADER, "shader/frag.glsl");
    program_ = std::make_unique<Program>(vert.get(), frag.get());
    program_->link();
    program_->use();
    //
    auto projection_ = glm::perspective(glm::radians(45.0f), 1.33f, 0.1f, 100.0f);

    program_->setMat4("projection", projection_);

    auto translate = glm::vec3(-2.f, 0.f, 0.f);
    this->object_ = std::make_unique<UIMovingObject>(glm::translate(glm::mat4(1.f), -translate));
    this->object2_ = std::make_unique<UIMovingObject>(glm::translate(glm::mat4(1.f), translate));

    this->loader_ = std::make_unique<ObjLoader>();

    auto sphere = std::make_shared<Sphere>(program_, false);

    object_->setDrawable(std::static_pointer_cast<Drawable>(sphere));

    auto drawable = loader_->load(modelPath);
    drawable->setProgram(program_);
    object2_->setDrawable(drawable);

    container_ = std::make_unique<Container>(glm::scale(glm::mat4(1.f), glm::vec3(5.f)));
    drawable = std::make_shared<ContainerDrawable>(program_);
    container_->setDrawable(drawable);

    // position = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.f, float pitch = 0
//    this->camera_ = std::make_unique<Camera>(-20.f, 20.f, 5.f,
//                                             0.f, 1.f, 0.f,
//                                             -45.f, -45.f);
    this->camera_ = std::make_unique<Camera>(0.f, 20.f, 20.f, 0.f, 1.f, -1.f, -90.f, -45.f);
}

void UIApplication::runLoop() {
    lastTime_ = glfwGetTime();

    while (!glfwWindowShouldClose(window_)) {
        currentTime_ = glfwGetTime();
        auto delta = currentTime_ - lastTime_;

        // input
        processInput();
#if AUDIO_ENABLED
        processAudio();
#endif
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        program_->setMat4("view", camera_->genViewMatrix());

        object_->update(delta);
        object2_->update(delta);

        object_->render();
        object2_->render();
        container_->render();

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
    } else if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {

    }
}

void UIApplication::terminate() {
    soundio_instream_destroy(instream_);
    soundio_device_unref(device_);
    soundio_destroy(soundio_);

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

void UIApplication::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        instance__.object_->bounce();
        instance__.object2_->bounce();
    }
}

void UIApplication::initSoundIo() {
    static int prioritized_sample_rates[] = {
        48000,
        44100,
        96000,
        24000,
        0,
    };


    soundio_ = soundio_create();
    if (!soundio_) {
        std::cerr << "out of memory" << std::endl;
        exit(-1);
    }

    int err = soundio_connect(soundio_);

    if (err) {
        std::cerr << "error connecting: " << soundio_strerror(err) << std::endl;
        exit(-1);
    }

    soundio_flush_events(soundio_);

    int default_in_device_index = soundio_default_input_device_index(soundio_);
    if (default_in_device_index < 0) {
        std::cerr << "no output device found" << std::endl;
        exit(-1);
    }

    device_ = soundio_get_input_device(soundio_, default_in_device_index);
    if (!device_) {
        std::cerr << "out of memory" << std::endl;
        exit(-1);
    }

    std::cout << "Output device: " << device_->name << std::endl;

    soundio_device_sort_channel_layouts(device_);

    int sample_rate = 0;
    int *sample_rate_ptr;
    for (sample_rate_ptr = prioritized_sample_rates; *sample_rate_ptr; sample_rate_ptr += 1) {
        if (soundio_device_supports_sample_rate(device_, *sample_rate_ptr)) {
            sample_rate = *sample_rate_ptr;
            std::cout << sample_rate << std::endl;
            break;
        }
    }
    if (!sample_rate)
        sample_rate = device_->sample_rates[0].max;

    sampleRate_ = sample_rate;

    enum SoundIoFormat fmt = SoundIoFormatFloat32NE;

    if (fmt == SoundIoFormatInvalid)
        fmt = device_->formats[0];

    instream_ = soundio_instream_create(device_);
    if (!instream_) {
        std::cerr << "out of memory" << std::endl;
        exit(-1);
    }
    instream_->format = fmt;
    instream_->sample_rate = sample_rate;
    instream_->read_callback = read_callback;
    instream_->overflow_callback = overflow_callback;
    instream_->userdata = &rc;

    if ((err = soundio_instream_open(instream_))) {
        std::cout << "unable to open input stream: " << soundio_strerror(err) << std::endl;
        exit(-1);
    }

    std::cout << instream_->layout.name << ' ' << sample_rate << "Hz " << soundio_format_string(fmt) << " interleaved"
              << std::endl;


    const int ring_buffer_duration_seconds = 30;
    int capacity = ring_buffer_duration_seconds * instream_->sample_rate * instream_->bytes_per_frame;
    rc.ring_buffer = soundio_ring_buffer_create(soundio_, capacity);
    if (!rc.ring_buffer) {
        std::cerr << "out of memory" << std::endl;
        exit(-1);
    }

    if ((err = soundio_instream_start(instream_))) {
        std::cerr << "unable to start input device: %s" << soundio_strerror(err) << std::endl;
        exit(-1);
    }
}


void UIApplication::read_callback(struct SoundIoInStream *instream, int frame_count_min, int frame_count_max) {
    RecordContext *rc = (RecordContext *) instream->userdata;
    struct SoundIoChannelArea *areas;
    int err;

    char *write_ptr = soundio_ring_buffer_write_ptr(rc->ring_buffer);
    int free_bytes = soundio_ring_buffer_free_count(rc->ring_buffer);
    int free_count = free_bytes / instream->bytes_per_frame;

    if (free_count < frame_count_min) {
        fprintf(stderr, "ring buffer overflow\n");
        exit(1);
    }

    int write_frames = std::min(free_count, frame_count_max);
    int frames_left = write_frames;

    for (;;) {
        int frame_count = frames_left;

        if ((err = soundio_instream_begin_read(instream, &areas, &frame_count))) {
            fprintf(stderr, "begin read error: %s", soundio_strerror(err));
            exit(1);
        }

        if (!frame_count)
            break;

        if (!areas) {
            // Due to an overflow there is a hole. Fill the ring buffer with
            // silence for the size of the hole.
            memset(write_ptr, 0, frame_count * instream->bytes_per_frame);
        } else {
            for (int frame = 0; frame < frame_count; frame += 1) {
                for (int ch = 0; ch < instream->layout.channel_count; ch += 1) {
                    memcpy(write_ptr, areas[ch].ptr, instream->bytes_per_sample);
                    areas[ch].ptr += areas[ch].step;
                    write_ptr += instream->bytes_per_sample;
                }
            }
        }

        if ((err = soundio_instream_end_read(instream))) {
            std::cerr << "end read error: " << soundio_strerror(err) << std::endl;
            exit(-1);
        }

        frames_left -= frame_count;
        if (frames_left <= 0)
            break;
    }

    int advance_bytes = write_frames * instream->bytes_per_frame;
    soundio_ring_buffer_advance_write_ptr(rc->ring_buffer, advance_bytes);
}

void UIApplication::processAudio() {
    static int nSamples = 0;
    static float sum = 0.f;
    static float threshold = -1;
    const int WindowSize = sampleRate_ / 2; // 0.5s
    soundio_flush_events(soundio_);
    int fill_bytes = soundio_ring_buffer_fill_count(rc.ring_buffer);
    char *read_buf = soundio_ring_buffer_read_ptr(rc.ring_buffer);
    soundio_ring_buffer_advance_read_ptr(rc.ring_buffer, fill_bytes);

    int nSamples_ = fill_bytes / sizeof(float);
    if (!nSamples_) return;

    nSamples += nSamples_;
    float *samples = (float *) read_buf;

    for (int i = 0; i < nSamples; i++) {
        sum += std::abs(samples[i]);
    }

    if (nSamples >= WindowSize) {
        float ave = sum / nSamples;
        nSamples = 0;
        sum = 0.f;

        if (threshold < 0) {
            threshold = ave;
        }
        else if (ave > 10 * threshold) {
            object_->bounce();
            object2_->bounce();
        } else {
            threshold = (threshold + ave) / 2.f;
        }
    }
}

void UIApplication::overflow_callback(struct SoundIoInStream *instream) {
    static int count = 0;
    std::cerr << "overflow " << ++count << std::endl;
}