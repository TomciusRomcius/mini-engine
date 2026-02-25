cmake_minimum_required(VERSION 4.1)

set(CMAKE_CXX_STANDARD 20)
project(mini-engine)
find_package(OpenGL REQUIRED)
find_package(GLEW)
add_subdirectory(core)
add_subdirectory(engine)
