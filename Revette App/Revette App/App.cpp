#include "App.h"
#include <iostream>


App::App()
{
    mainWindow = nullptr;
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
    mainWindow = glfwCreateWindow(800, 600, "Revette", NULL, NULL);
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
    glViewport(0, 0, 800, 600);

    // Load shader program
    shader = std::make_unique<Shader>("basic.vs", "basic.fs");

    if (!tilemap.loadChunks()) return false;

    return true;
}



void App::processInput()
{
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mainWindow, true);
    }
}



void App::render()
{
    //Rendering commands
    glClearColor(0.0f, 0.08f, 0.14f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render the frame
    // Draw the tilemap
    if (!tilemap.drawChunks(shader))
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
