#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "Shader.h"
#include "TileMap.h"

class App
{
public:
	App();
	~App();
	int run();
private:
	void cleanup();
	bool init();
	void processInput();
	void render();

	GLFWwindow* mainWindow;
	std::unique_ptr<Shader> shader;

	TileMap tilemap;
};

