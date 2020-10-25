#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "App.h"



App::App()
{
    mainWindow = nullptr;
    camera = Camera();
}



App::~App()
{

}



void App::cleanup()
{
    glfwTerminate();
}



bool App::init()
{
    // Load glfw and configure it
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a window object
    mainWindow = glfwCreateWindow(1500, 1000, "Revette", NULL, NULL);
    if (mainWindow == NULL) {
        std::cout << "Error creating window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(mainWindow);
    // Load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        return false;
    }
    glViewport(0, 0, 1500, 1000);

    // Load shader program
    shader = std::make_unique<Shader>("basic.vs", "basic.fs");

    // Load texture atlas
    if (!textureAtlas.loadTexture("texture_atlas.png"))
    {
        std::cout << "Failed to load texture atlas" << std::endl;
        return false;
    }

    camera.setPosition(0.0f, 1.0f);

    if (!tilemap.loadChunks()) return false;

    return true;
}



void App::processInput()
{
    // Check for exit
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mainWindow, true);
    }
    
    float cameraSpeed = 1.5f;

    //Movement
    if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.translate(0.0f, -cameraSpeed);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.translate(0.0f, cameraSpeed);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.translate(-cameraSpeed, 0.0f);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.translate(cameraSpeed, 0.0f);
    }
}



void App::render()
{
    //Rendering commands
    glClearColor(0.0f, 0.08f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    constexpr float zoomFactor = 30.0f;

    //Calculate the projection matrix
    glm::mat4 projection = glm::ortho(0.0f, 1500.0f / zoomFactor, 1000.0f / zoomFactor, 0.0f, -1.0f, 1.0f);

    //Calculate vertex offset due to camera position
    glm::vec2 cameraPosition = camera.getPosition();
    glm::vec2 cameraOffset = cameraPosition * -1.0f;

    textureAtlas.bindTexture();

    // Render the frame
    // Draw the tilemap
    if (!tilemap.drawChunks(shader, projection, cameraOffset))
    {
        std::cout << "Error drawing tilemap" << std::endl;
    }

    // Swap buffers
    glfwSwapBuffers(mainWindow);
}



int App::run()
{
    if (!init()) {
        return -1;
    }
    while (!glfwWindowShouldClose(mainWindow)) {
        processInput();
        render();
        glfwPollEvents();
    }

    cleanup();
    return 0;
}
