#pragma once



class Texture
{
public:
	void bindTexture();
	bool loadTexture(const char* texturePath);

	unsigned int textureID = 0;
};

