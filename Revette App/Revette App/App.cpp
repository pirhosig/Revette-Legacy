#include <thread>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "App.h"
#include "Logging/GlobalAppLog.h"



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
    GlobalAppLog.writeLog("Loading GLFW", LOGMODE::INFO);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // Create a window object
    GlobalAppLog.writeLog("Creating window", LOGMODE::INFO);
    mainWindow = glfwCreateWindow(1500, 1000, "Revette", NULL, NULL);
    if (mainWindow == NULL) {

        GlobalAppLog.writeLog("Error creating window", LOGMODE::FATAL);
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mainWindow);
    glfwSetWindowUserPointer(mainWindow, this);


    // Load opengl function pointers
    GlobalAppLog.writeLog("Loading opengl function pointers", LOGMODE::INFO);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        GlobalAppLog.writeLog("Failed to initialize glad", LOGMODE::FATAL);
        return false;
    }


    // Set the callback functions for various input events
    glfwSetScrollCallback(mainWindow, App::scrollwheelCallbackWrapper);

    glViewport(0, 0, 1500, 1000);

    // Load shader programs
    chunkShader  = std::make_unique<Shader>("./Assets/Shaders/chunk.vs",  "./Assets/Shaders/chunk.fs" );
    entityShader = std::make_unique<Shader>("./Assets/Shaders/entity.vs", "./Assets/Shaders/entity.fs");

    // Load texture atlas
    if (!textureAtlas.loadTexture("./Assets/Textures/texture_atlas.png"))
    {
        GlobalAppLog.writeLog("Failed to load texture atlas", LOGMODE::ERROR);
        return false;
    }
    
    player.setPosition(0.0f, 100.0f);

    if (!tilemap.loadChunks()) return false;
    
    return true;
}



// This function is called in the main application loop.
// This is where events in the application other than input and rendering should be processed.
void App::loop()
{
    // Set the camera to the player's position
    camera.setPosition(player.x, player.y);
}



void App::processInput()
{
    // Call glfw callbacks
    glfwPollEvents();

    // Check for exit
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mainWindow, true);
    }
    
    const float cameraSpeed = 1.5f;

    // Player movement
    if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        player.move(0.0f, -cameraSpeed);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        player.move(0.0f, cameraSpeed);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        player.move(-cameraSpeed, 0.0f);
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        player.move(cameraSpeed, 0.0f);
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

    // Render the frame
    // Draw the tilemap
    if (!tilemap.drawChunks(chunkShader, textureAtlas, projection, cameraOffset))
    {
        GlobalAppLog.writeLog("Failed to draw tilemap", LOGMODE::ERROR);
    }
    
    // Swap buffers
    glfwSwapBuffers(mainWindow);
}



// Runs the application
int App::run()
{
    if (!init()) {
        GlobalAppLog.writeLog("Initialization failed", LOGMODE::FATAL);
        return -1;
    }
    while (!glfwWindowShouldClose(mainWindow)) {
        // Get input
        processInput();
        // Process events
        loop();
        // Render frame
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(6));
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
