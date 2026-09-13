#include "Application.h"

#include <memory>
#include <stdexcept>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Debugging.h"
#include "components/Light.h"
#include "components/Mesh.h"
#include "components/MeshRenderer.h"
#include "components/SceneCamera.h"
#include "components/Transform.h"
#include "ecs/Scene.h"
#include "layers/Layer.h"
#include "layers/PropertiesSidebar.h"
#include "layers/SceneEntitiesSidebar.h"
#include "layers/ViewportLayer.h"

namespace mini_engine {
    Application::Application() {
        if (!glfwInit()) {
            throw std::runtime_error("GLFW initialization failed");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow *window = glfwCreateWindow(1280, 720, "Mini engine", nullptr, nullptr);
        if (window == nullptr) {
            throw std::runtime_error("Failed to create GLFW window");
        }
        glfwMakeContextCurrent(window);
        GLenum err = glewInit();
        if (err != GLEW_OK && err != GLEW_ERROR_NO_GLX_DISPLAY) {
            throw std::runtime_error("glewInit() failed");
        }

        glCall(glEnable(GL_DEPTH_TEST));

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        Scene scene;
        Entity *selectedEntity = nullptr;

        Entity *cameraEntity = scene.createEntity("Camera");
        auto *cameraTransform = new Transform();
        cameraTransform->position = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraEntity->getECSContainer()->addComponent(*cameraEntity, cameraTransform);

        auto *camera = new SceneCamera();
        camera->setWindow(window);
        cameraEntity->getECSContainer()->addComponent(*cameraEntity, camera);

        Entity *cubeEntity = scene.createEntity("Cube");
        auto *cubeTransform = new Transform();
        cubeEntity->getECSContainer()->addComponent(*cubeEntity, cubeTransform);

        auto *cube = new Mesh();
        cube->vertices = {
            // Front (+Z)
            {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f},
            // Back (-Z)
            { 0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f},
            // Left (-X)
            {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f, -0.5f},
            // Right (+X)
            { 0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { 0.5f,  0.5f,  0.5f},
            // Top (+Y)
            {-0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { 0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            // Bottom (-Y)
            {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f,  0.5f}, {-0.5f, -0.5f,  0.5f},
        };
        cube->normals = {
            // Front
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
            // Back
            {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
            // Left
            {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
            // Right
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            // Top
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            // Bottom
            {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
        };
        cube->indices = {
            0,  1,  2,  0,  2,  3,
            4,  5,  6,  4,  6,  7,
            8,  9, 10,  8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23,
        };
        cubeEntity->getECSContainer()->addComponent(*cubeEntity, cube);
        cubeEntity->getECSContainer()->addComponent(*cubeEntity, new MeshRenderer());

        Entity *lightEntity = scene.createEntity("Light");
        auto *lightTransform = new Transform();
        lightTransform->position = glm::vec3(1.5f, 1.5f, 1.5f);
        lightEntity->getECSContainer()->addComponent(*lightEntity, lightTransform);

        auto *light = new Light();
        light->color = glm::vec3(1.0f, 0.95f, 0.85f);
        light->intensity = 1.5f;
        light->lightType = "point";
        light->light = std::make_unique<PointLight>();
        light->removePropertyGroupIfExists("light");
        light->addPropertyGroup("light", light->light->getProperties());
        lightEntity->getECSContainer()->addComponent(*lightEntity, light);

        selectedEntity = cubeEntity;

        std::vector<std::unique_ptr<Layer>> layers;
        layers.push_back(std::make_unique<SceneEntitiesSidebar>(scene, selectedEntity));
        layers.push_back(std::make_unique<PropertiesSidebar>(selectedEntity));
        layers.push_back(std::make_unique<ViewportLayer>(scene));

        for (auto &layer: layers) {
            layer->onAttach();
        }

        scene.start();

        while (!glfwWindowShouldClose(window)) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::DockSpaceOverViewport();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            for (auto &layer: layers) {
                layer->update();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
