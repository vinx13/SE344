cmake_minimum_required(VERSION 3.8)

project(SE344)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

add_subdirectory(deps/glfw)
add_subdirectory(deps/gl3w)
add_subdirectory(deps/glm)
add_subdirectory(deps/libsoundio)
find_package(glm REQUIRED)
file(GLOB SOURCE_FILES src/*.cpp src/*.h)

include_directories(${glfw_INCLUDE_DIRS} ${libsoundio_SOURCE_DIR} deps/glm)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gl3w glfw libsoundio_shared ${LIBSOUNDIO_LIBS} ${LIBM} ${OPENGL_LIBRARIES})

file(COPY src/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
