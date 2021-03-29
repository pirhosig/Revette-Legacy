#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Entities/Player.h"
#include "Rendering/Shader.h"
#include "Rendering/TextureArray.h"
#include "TileMap.h"



class App
{
public:
	App();
	~App();
	int run();

	// Opengl callback functions
	static void scrollwheelCallbackWrapper(GLFWwindow* window, double xOffset, double yOffset);
	void scrollwheelCallback(double yOffset);
private:
	void cleanup();
	bool init();
	bool loadTextures();

	void loop();
	void processInput(const double frameTime);
	void render();

	GLFWwindow* mainWindow;
	std::unique_ptr<Shader> chunkShader;
	std::unique_ptr<Shader> entityShader;

	unsigned int frameTime;

	Camera camera;
	Player player;
	TileMap tilemap;
	TextureArray textureAtlas;
};

