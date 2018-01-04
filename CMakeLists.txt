cmake_minimum_required(VERSION 3.8)

project(SE344)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenGL REQUIRED)

add_subdirectory(deps/glfw)
add_subdirectory(deps/gl3w)
add_subdirectory(deps/glm)
add_subdirectory(deps/libsoundio)
add_subdirectory(deps/SOIL)
find_package(glm REQUIRED)
find_package(freetype REQUIRED)

file(GLOB SOURCE_FILES src/*.cpp src/*.h)
file(GLOB SHADER_FILES src/shader/*)
file(COPY resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${glfw_INCLUDE_DIRS} ${libsoundio_SOURCE_DIR} ${FREETYPE_INCLUDE_DIRS} deps/glm deps/SOIL/src)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gl3w glfw libsoundio_shared ${LIBSOUNDIO_LIBS} ${LIBM} ${OPENGL_LIBRARIES} SOIL_Static freetype)

#add_custom_target(copy_shader ALL
#        COMMAND cmake -E copy_directory src/shader ${CMAKE_BINARY_DIR}/shader)
#add_dependencies(${PROJECT_NAME} copy_shader)

# Scan through resource folder for updated files and copy if none existing or changed
foreach(shader_file ${SHADER_FILES})
    get_filename_component(filename ${shader_file} NAME)

    set(output "${CMAKE_CURRENT_BINARY_DIR}/shader/${filename}")

    add_custom_command(
            COMMENT "Moving updated shader-file '${filename}'"
            OUTPUT ${output}
            DEPENDS ${shader_file}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${shader_file}
            ${output}
    )
    add_custom_target(${filename} ALL DEPENDS ${shader_file} ${output})
    add_dependencies(${PROJECT_NAME} ${filename})
endforeach()