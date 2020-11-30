#include <iostream>
#include <thread>
#include <chrono>

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



// Clean up resources upon application termination
void App::cleanup()
{
    glfwTerminate();
}



// Load and/or configure everything that requires it
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
    glfwSetWindowUserPointer(mainWindow, this);

    // Load opengl function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to init glad" << std::endl;
        return false;
    }

    // Set the callback functions for various input events
    glfwSetScrollCallback(mainWindow, App::scrollwheelCallbackWrapper);

    glViewport(0, 0, 1500, 1000);

    // Load shader program
    shader = std::make_unique<Shader>("basic.vs", "basic.fs");

    // Load texture atlas
    if (!textureAtlas.loadTexture("texture_atlas.png"))
    {
        std::cout << "Failed to load texture atlas" << std::endl;
        return false;
    }
    
    camera.setPosition(0.0f, 100.0f);

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



// The rendering function, called every frame
void App::render()
{
    //Rendering commands
    glClearColor(0.29f, 0.643f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Calculate the projection matrix
    glm::mat4 projection = glm::ortho(0.0f, 1500.0f / camera.zoomFactor, 1000.0f / camera.zoomFactor, 0.0f, -1.0f, 1.0f);

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



// Runs the application
int App::run()
{
    if (!init()) {
        return -1;
    }
    while (!glfwWindowShouldClose(mainWindow)) {
        processInput();
        render();
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    cleanup();
    return 0;
}



// Callback wrapper for mouse scrolling since member since only static class functions can be used as a callback
// Calls the member function of the class instance set to manage the window
void App::scrollwheelCallbackWrapper(GLFWwindow* window, double xOffset, double yOffset)
{
    App* appInstance = static_cast<App*>(glfwGetWindowUserPointer(window));
    appInstance->scrollwheelCallback(yOffset);
}


// Callback function for mouse scrolling
void App::scrollwheelCallback(double yOffset)
{
    camera.zoomFactor += static_cast<float>(yOffset) / 2.0f;
}
