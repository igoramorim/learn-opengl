#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

const unsigned int SCREEN_WIDTH = 800;
const unsigned int SCREEN_HEIGHT = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	wWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Glad load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	// build and compile our shader program
	Shader shader("shaders/texture.vs", "shaders/texture.fs");

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

	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Setup triangle
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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
	unsigned int texture1;
	glGenTextures(1,			// number of textures to be generated
		          &texture1);	// array of int that stores the generated textures IDs
	glBindTexture(GL_TEXTURE_2D, texture1);	// all upcoming GL_TEXTURE_2D operations now have effect on this texture object
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
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
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
	shader.use(); // don't forget to activate/use the shader before setting uniforms!
	// either set it manually like so:
	// glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
	// or set it via the shader class
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0); // active texture 0
		glBindTexture(GL_TEXTURE_2D, texture1); // bind the activated texture
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		float time = glfwGetTime();
		shader.use(); // render the triangle
		shader.setFloat("time", time); // set the uniform shader variable

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window); // Swap the front buffer and the back buffer
		glfwPollEvents(); // Checks if any events are triggered (like keyboard)
	}

	// De-allocate all resources once they were already used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}