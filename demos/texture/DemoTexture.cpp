#include "DemoTexture.h"

#include "../../stb_image.h"

namespace demo {

	DemoTexture::DemoTexture() :
		m_Shader { "shaders/texture.vs", "shaders/texture.fs" }
	{
		// Vertex input data
		float vertices[] = {
			// positions         // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		// Setup triangle
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture Coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		// load and create texture1
		glGenTextures(1,			// number of textures to be generated
			&m_Texture1);	// array of int that stores the generated textures IDs
		glBindTexture(GL_TEXTURE_2D, m_Texture1);	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// when downscaling
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// when upscaling

		int width, height, nrChannels;
		// load image data
		unsigned char* imageData = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);
		if (imageData)
		{
			glTexImage2D(GL_TEXTURE_2D,	// specifies the texture target that was bind with glBindTexture(GL_TEXTURE_2D, ...) previously
				0,						// specifies the mipmap level if you want to set manually. 0 is the base level
				GL_RGB,					// the format we want OpenGL to store our texture
				width,					// width of the the texture
				height,					// height of the texture
				0,						// dont know what it is. set 0 always
				GL_RGB,					// format of the source image
				GL_UNSIGNED_BYTE,		// datatype wich the image was loaded in. we loaded the image(imageData) as chars(bytes)
				imageData);				// image data

			glGenerateMipmap(GL_TEXTURE_2D); // generate all mipmaps levels for the active binded texture
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imageData); // free image memory

		// load and create texture2
		glGenTextures(1, &m_Texture2);
		glBindTexture(GL_TEXTURE_2D, m_Texture2);
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
		imageData = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
		if (imageData)
		{
			// GL_RGBA because this texture is png
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(imageData);

		// tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
		m_Shader.use(); // don't forget to activate/use the shader before setting uniforms!
		// either set it manually like so:
		// glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
		// or set it via the shader class
		m_Shader.setInt("texture1", 0);
		m_Shader.setInt("texture2", 1);
	}

	DemoTexture::~DemoTexture()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
	}

	void DemoTexture::OnUpdate(float deltaTime)
	{

	}

	void DemoTexture::OnRender()
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0); // active texture 0
		glBindTexture(GL_TEXTURE_2D, m_Texture1); // bind the activated texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_Texture2);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void DemoTexture::OnImGuiRender()
	{

	}

}
