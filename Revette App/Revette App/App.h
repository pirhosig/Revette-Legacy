#pragma once
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
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
	void processInput();
	void render();

	GLFWwindow* mainWindow;
	std::unique_ptr<Shader> shader;

	Texture textureAtlas;
	TileMap tilemap;
	Camera camera;
};

