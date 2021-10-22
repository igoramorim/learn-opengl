#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "stb_image.h"
#include "shader.h"

class Texture
{
public:
	unsigned int ID;
	GLenum type;
	GLenum slot;

	// TODO: remove sourceFormat and destFormat from constructor. We can retrieve the channels from with stbi_load() and GL_RED(1) or GL_RGB(3) or GL_RGBA(4)
	Texture(const char* imagePath, GLenum texType, GLenum slotType, GLenum sourceFormat, GLenum destFormat, GLenum pixelType)
	{
		type = texType;		// assigns the type of the texture to the texture object
		slot = slotType;	// assigns the slot type of the texture

		// TODO: Add constructor param to set whether flip the image vertically or not
		// stbi_set_flip_vertically_on_load(true);

		// generate an OpenGL texture object
		glGenTextures(1, &ID);
		// glActiveTexture(slot);

		// bind the texture. all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		bind();

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // when downscaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // when upscaling

		// stores the width, height, and the number of color channels of the image
		int widthImg, heightImg, nrChannels;

		// load image data in bytes
		unsigned char* imageData = stbi_load(imagePath, &widthImg, &heightImg, &nrChannels, 0);

		if (imageData)
		{
			/*GLenum format;
			if (nrChannels == 1)
				format = GL_RED;
			else if (nrChannels == 3)
				format = GL_RGB;
			else if (nrChannels == 4)
				format = GL_RGBA;*/

			glTexImage2D(texType,		// specifies the texture target that was bind with glBindTexture(GL_TEXTURE_2D, ...) previously
				0,						// specifies the mipmap level if you want to set manually. 0 is the base level
				destFormat,				// the format we want OpenGL to store our texture
				widthImg,				// width of the the texture
				heightImg,				// height of the texture
				0,						// dont know what it is. set 0 always
				sourceFormat,			// format of the source image
				pixelType,				// datatype wich the image was loaded in. we loaded the image(imageData) as chars(bytes)
				imageData);				// image data

			glGenerateMipmap(texType); // generate all mipmaps levels for the active binded texture
		}
		else
		{
			std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE" << std::endl;
		}

		stbi_image_free(imageData); // free image memory

		// unbinds the OpenGL Texture object so that it can't accidentally be modified
		unbind();
	}

	void texUnit(Shader& shader, const char* uniform, GLuint unit)
	{
		// Shader needs to be activated before changing the value of a uniform
		shader.use();
		// Sets the value of the uniform
		shader.setInt(uniform, unit);
	}

	void activeAndBind()
	{
		glActiveTexture(slot);
		bind();
	}

	void bind()
	{
		glBindTexture(type, ID);
	}

	void unbind()
	{
		glBindTexture(type, 0);
	}

	void Delete()
	{
		glDeleteTextures(1, &ID);
	}

	// TODO: function to set the wrapping and filtering options that was set default values in constructor
};

#endif
