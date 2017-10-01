cmake_minimum_required(VERSION 3.8)

project(SE344)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

add_subdirectory(deps/glfw)
add_subdirectory(deps/gl3w)
add_subdirectory(deps/glm)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)
include_directories(${glfw_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/uiobject.cpp src/uiobject.h)
target_link_libraries(${PROJECT_NAME} gl3w glfw ${OPENGL_LIBRARIES})

file(COPY src/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
