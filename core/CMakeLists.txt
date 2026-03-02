include(FetchContent)

add_library(core STATIC src/VBO.cpp src/VBO.h
        src/VAO.cpp
        src/VAO.h
        src/AttributeMemory.cpp
        src/AttributeMemory.h
        src/Shader.cpp
        src/Shader.h
        src/Program.cpp
        src/Program.h
        src/Debugging.cpp
        src/Debugging.h
)

FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

target_link_libraries(core PUBLIC OpenGL::GL GLEW::GLEW glfw)

add_custom_command(
        TARGET core POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
)