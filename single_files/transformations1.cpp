#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#include <iostream>

#include "shader.h"
#include "texture.h"

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
	Shader shader("shaders/transformation.vs", "shaders/transformation.fs");

	// Vertex input data
	float vertices[] = {
		// positions         // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
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

	Texture woodTex("textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE);
	woodTex.texUnit(shader, "texture1", 0);

	Texture smileTex("textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	smileTex.texUnit(shader, "texture2", 1);

	shader.use();

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		woodTex.activeAndBind();
		smileTex.activeAndBind();

		float time = glfwGetTime();

		/* Transformation 1
		* ===================================================================================
		*/
		glm::mat4 transform = glm::mat4(1.0f); // identity matrix
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
		shader.setMat4("transform", transform);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ==================================================================================

		/* Transformation 2
		* ===================================================================================
		*/
		transform = glm::mat4(1.0f); // reset to identity matrix
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f));
		transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
		float s = sin(time) * 0.5 + 0.5;
		transform = glm::scale(transform, glm::vec3(s, s, 1.0f));
		shader.setMat4("transform", transform);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ==================================================================================

		glfwSwapBuffers(window); // Swap the front buffer and the back buffer
		glfwPollEvents(); // Checks if any events are triggered (like keyboard)
	}

	// De-allocate all resources once they were already used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	// woodTex.Delete();
	// smileTex.Delete();

	glfwTerminate();
	return 0;
}