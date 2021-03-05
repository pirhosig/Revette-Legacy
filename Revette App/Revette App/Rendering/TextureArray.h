#pragma once
#include <glad/glad.h>



class TextureArray
{
public:
	void bindTexture();
	bool loadTexture(const char* texturePath);

	GLuint textureID = 0;
};

