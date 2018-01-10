set(GLFW_BUILD_EXAMPLES FALSE)
set(GLFW_BUILD_TESTS FALSE)
set(GLFW_BUILD_DOCS FALSE)
add_subdirectory(glfw)

add_subdirectory(gl3w)

add_subdirectory(glm)


set(BUILD_STATIC_LIBS FALSE)
set(BUILD_EXAMPLE_PROGRAMS FALSE)
set(BUILD_TESTS FALSE)
add_subdirectory(libsoundio)

add_subdirectory(SOIL)