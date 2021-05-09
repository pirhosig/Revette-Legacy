#include <chrono>
#include <fstream>
#include <math.h>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "App.h"
#include "Logging/GlobalAppLog.h"



App::App()
{
    mainWindow = nullptr;
    camera = Camera();

    // Set default fps to 60
    frameTime = 16;
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

    // Create a window object and bind it as the current window
    GlobalAppLog.writeLog("Creating window", LOGMODE::INFO);
    mainWindow = glfwCreateWindow(1920, 1080, "Revette", glfwGetPrimaryMonitor(), NULL);
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

    // Load settings from file
    std::ifstream settingFile;
    settingFile.open("./Assets/Settings.txt");
    if (!settingFile.is_open())
    {
        GlobalAppLog.writeLog("Failed to load settings", LOGMODE::ERROR);
        return false;
    }
    // Extract settings
    unsigned int frameRate;
    settingFile >> frameRate;
    frameTime = 1000 / frameRate;
    int worldSeed;
    settingFile >> worldSeed;
    tilemap.terrainGenerator->setSeed(worldSeed);
    //Close file object
    settingFile.close();

    // Set the callback functions for various input events
    glfwSetScrollCallback(mainWindow, App::scrollwheelCallbackWrapper);
    // Set the viewport to properly scale rendered scene to screen size
    glViewport(0, 0, 1920, 1080);

    // Load shader programs
    chunkShader  = std::make_unique<Shader>("./Assets/Shaders/chunk.vs",  "./Assets/Shaders/chunk.fs" );
    entityShader = std::make_unique<Shader>("./Assets/Shaders/entity.vs", "./Assets/Shaders/entity.fs");
    //Load textures
    if (!loadTextures()) return false;

    player.setPosition(50.0f, 100.0f);

    // Load and generate chunks
    if (!tilemap.loadChunks()) return false;

    return true;
}



// Loads all textures from files
bool App::loadTextures()
{
    // Load texture atlas
    if (!textureAtlas.loadTexture("./Assets/Textures/texture_atlas.png"))
    {
        GlobalAppLog.writeLog("Failed to load texture atlas", LOGMODE::ERROR);
        return false;
    }

    // Load player texture
    player.loadTexture("./Assets/Textures/player.png");

    return true;
}



// This function is called in the main application loop.
// This is where events in the application other than input and rendering should be processed.
void App::loop()
{
    // Set the camera to the player's position
    camera.setPosition(player.x, player.y);
}



void App::processInput(const double frameTime)
{
    // Call glfw callbacks
    glfwPollEvents();

    // Check for exit
    if (glfwGetKey(mainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(mainWindow, true);
    }
    

    // Use change in time to control speed, so that the game runs independant of frame rate
    const float cameraSpeed = 30.0f * static_cast<float>(frameTime);


    // Player movement
    glm::vec2 playerMovement(0.0f, 0.0f);
    bool playerHasMoved = false;

    if (glfwGetKey(mainWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        playerMovement.y += -cameraSpeed;
        playerHasMoved = true;
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        playerMovement.y += cameraSpeed;
        playerHasMoved = true;
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        playerMovement.x += -cameraSpeed;
        playerHasMoved = true;
    }
    if (glfwGetKey(mainWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        playerMovement.x += cameraSpeed;
        playerHasMoved = true;
    }

    if (playerHasMoved)
    {
        player.move(tilemap, playerMovement.x, playerMovement.y);
    }

    // World interaction
    if (glfwGetMouseButton(mainWindow, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(mainWindow, &mouseX, &mouseY);
        GlobalAppLog.writeLog("Click: " + std::to_string(mouseX) + " " + std::to_string(mouseY), LOGMODE::INFO);

        unsigned int tx = static_cast<unsigned int>(player.x);
        unsigned int ty = static_cast<unsigned int>(player.y);
        tilemap.setTile(tx, ty, { 0, 0 });
    }
}



// The rendering function, called every frame
void App::render()
{
    //Rendering commands
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.29f, 0.643f, 0.85f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Calculate the projection matrix
    const float zoom = camera.zoomFactor;
    glm::mat4 projection = glm::ortho(-960.0f / zoom, 960.0f / zoom, 540.0f / zoom, -540.0f / zoom, -1.0f, 1.0f);
    
    //Calculate vertex offset due to camera position
    glm::vec2 cameraPosition = camera.getPosition();
    glm::vec2 cameraOffset = cameraPosition * -1.0f;

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(cameraOffset, 0.0f));

    glm::mat4 projectionView = projection * view;

    // Render the frame
    // Draw the tilemap
    if (!tilemap.drawChunks(chunkShader, textureAtlas, projectionView, glm::vec2(0.0f, 0.0f)))
    {
        GlobalAppLog.writeLog("Failed to draw tilemap", LOGMODE::ERROR);
    }

    // Draw the player
    if (!player.draw(entityShader, projectionView, glm::vec2(0.0f, 0.0f)))
    {
        GlobalAppLog.writeLog("Failed to draw player", LOGMODE::ERROR);
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

    // Measure time in between frames
    double lastFrameTime = glfwGetTime();

    while (!glfwWindowShouldClose(mainWindow)) {
        // Calculate when the next frame should render
        const auto frameBegin = std::chrono::steady_clock::now();
        const auto frameEnd = frameBegin + std::chrono::milliseconds(frameTime);

        // Calculate time since last frame
        double time = glfwGetTime();
        double deltaTime = time - lastFrameTime;
        lastFrameTime = time;

        // Get input
        processInput(deltaTime);
        // Process events
        loop();
        // Render frame
        render();

        std::this_thread::sleep_until(frameEnd);
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
    float zoom = camera.zoomFactor;
    zoom += static_cast<float>(yOffset) / 2.0f;
    if (zoom < 15.0f) zoom = 15.0f;
    else if (zoom > 50.0f) zoom = 50.0f;
    camera.zoomFactor = zoom;
}
