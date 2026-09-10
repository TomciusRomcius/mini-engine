add_executable(engine
        main.cpp
        Application.cpp
        Application.h)

target_link_libraries(engine PRIVATE core)

add_custom_command(
        TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/core/src/shaders
        $<TARGET_FILE_DIR:engine>/shaders
)
