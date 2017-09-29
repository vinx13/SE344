cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

project(gl3w)
set(GL3W_INCLUDE_DIRS gl3w/include)

add_library(${PROJECT_NAME} src/gl3w.c)