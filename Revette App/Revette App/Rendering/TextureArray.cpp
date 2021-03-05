#include "TextureArray.h"

#include <algorithm>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "../Logging/GlobalAppLog.h"



constexpr auto pixelFormat = GL_RGBA;
constexpr int TEXTURE_SIZE = 32;
constexpr int TEXTURE_PIXEL_BYTE_SIZE = 4;
constexpr int TEXTURE_MEMORY_SIZE = TEXTURE_SIZE * TEXTURE_SIZE * 4;



bool TextureArray::loadTexture(const char* texturePath)
{
	bool success = true;

	// Generate a texture object on the GPU and bind it
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load the texture atlas image file into a byte array
	int imageWidth, imageHeight, colourChannelCount;
	unsigned char* imageDataArray = stbi_load(texturePath, &imageWidth, &imageHeight, &colourChannelCount, 0);
	// Check if the texture atlas image was properly loaded
	if (!imageDataArray)
	{
		std::string errorMessage("Failed to load texture atlas from file: ");
		errorMessage += texturePath;
		GlobalAppLog.writeLog(errorMessage, LOGMODE::ERROR);
		return false;
	}

	// Calculate the height and width of the image in standard texture sizes, and then the number of textures
	int atlasWidth  = imageWidth  / TEXTURE_SIZE;
	int atlasHeight = imageHeight / TEXTURE_SIZE;
	int atlasTextureCount = atlasWidth * atlasHeight;

	// Allocate storage for the texture array
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, TEXTURE_SIZE, TEXTURE_SIZE, atlasTextureCount, 0, pixelFormat, GL_UNSIGNED_BYTE, NULL);

	// Load each individual texture into the texture array
	for (int j = 0; j < atlasHeight; ++j)
	{
		for (int i = 0; i < atlasWidth; ++i)
		{
			// Create a byte array to hold the specific texture which is read from the main atlas image
			unsigned char* textureDataArray = new unsigned char[TEXTURE_MEMORY_SIZE];

			// Copy the texture line by line from the atlas to the array
			int startLine = j * TEXTURE_SIZE;
			for (int lineCount = 0; lineCount < TEXTURE_SIZE; ++lineCount)
			{
				int linePosition = startLine + lineCount;
				unsigned char* lineBegin = imageDataArray + ((linePosition * imageWidth) + (i * TEXTURE_SIZE)) * TEXTURE_PIXEL_BYTE_SIZE;
				unsigned char* lineEnd = lineBegin + TEXTURE_SIZE * TEXTURE_PIXEL_BYTE_SIZE;
				unsigned char* textureDataPosition = textureDataArray + TEXTURE_SIZE * lineCount * TEXTURE_PIXEL_BYTE_SIZE;
				std::copy(lineBegin, lineEnd, textureDataPosition);
			}

			// Add the texture to the texture array
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, (j * atlasWidth + i), TEXTURE_SIZE, TEXTURE_SIZE, 1, pixelFormat, GL_UNSIGNED_BYTE, textureDataArray);

			// Delete the texture array since it has been loaded
			delete[] textureDataArray;
		}
	}

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	// Deallocate the memory occupied by the texture atlas image
	stbi_image_free(imageDataArray);

	return success;
}



// Set the texture as the currently bound texture object
void TextureArray::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}
