add_executable(engine
        main.cpp
        Application.cpp
        Application.h
        components/Transform.cpp
        components/Transform.h
        components/Mesh.h
        components/MeshRenderer.cpp
        components/MeshRenderer.h
        components/SceneCamera.cpp
        components/SceneCamera.h
        layers/Layer.h
        layers/ViewportLayer.cpp
        layers/ViewportLayer.h
        layers/SceneEntitiesSidebar.cpp
        layers/SceneEntitiesSidebar.h
        layers/PropertiesSidebar.cpp
        layers/PropertiesSidebar.h)

target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(engine PRIVATE core)

add_custom_command(
        TARGET engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/core/src/shaders
        $<TARGET_FILE_DIR:engine>/shaders
)
