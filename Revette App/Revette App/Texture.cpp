#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "Texture.h"


unsigned textureCount = 2;
const char* textureFiles[] =
{
	"dirt.png",
	"grass.png"
};
auto pixelFormat = GL_RGB;



bool Texture::loadTexture(const char* texturePath)
{
	bool success = true;

	// Generate a texture object on the GPU and bind it
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
	
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	
	// Allocate storage for the texture array
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGB, 32, 32, textureCount, 0, pixelFormat, GL_UNSIGNED_BYTE, NULL);

	// Load each texture into the array
	for (unsigned i = 0; i < textureCount; ++i)
	{
		// load and generate the texture
		int width, height, nrChannels;
		unsigned char* data = stbi_load(textureFiles[i], &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, 32, 32, 1, pixelFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
			success = false;
		}

		//Deallocate memory occupied by loaded image
		stbi_image_free(data);
	}

	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	return success;
}



// Set the texture as the currently bound texture object
void Texture::bindTexture()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureID);
}
