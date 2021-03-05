#include "Texture.h"

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "../Logging/GlobalAppLog.h"



// Set the texture as the currently bound texture object
void Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}



bool Texture::loadTexture(const char* textureFilePath, const GLenum imageType)
{
	bool success = true;

	//Create and bind texture object
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load the texture image file into a byte array
	int imageWidth, imageHeight, colourChannelCount;
	unsigned char* imageDataArray = stbi_load(textureFilePath, &imageWidth, &imageHeight, &colourChannelCount, 0);

	if (!imageDataArray)
	{
		std::string errorMessage("Failed to load texture from file: ");
		errorMessage += textureFilePath;
		GlobalAppLog.writeLog(errorMessage, LOGMODE::ERROR);
		return false;
	}

	// Allocate storage for texture on the GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, imageType, GL_UNSIGNED_BYTE, imageDataArray);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deallocate memory occupied by image object
	stbi_image_free(imageDataArray);

	return success;
}

