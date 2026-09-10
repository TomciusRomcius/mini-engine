add_library(core STATIC
        src/VBO.cpp
        src/VBO.h
        src/EBO.cpp
        src/EBO.h
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

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(core PUBLIC OpenGL::GL GLEW::GLEW glfw)
