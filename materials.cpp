#include <iostream>
#include <iterator> // std::size
#include <string>   // std::to_string

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "VertexBuffer.h";
#include "VertexArray.h";
#include "IndexBuffer.h";
#include "shader.h"
#include "texture.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};

// http://devernay.free.fr/cours/opengl/materials.html
const Material materials[] = {
	Material{ glm::vec3( 0.0215f,  0.1745f,  0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(   0.633f, 0.727811f,    0.633f), 128.0f *      0.6f }, // emerald
	Material{ glm::vec3(  0.135f,  0.2225f,  0.1575f), glm::vec3(   0.54f,    0.89f,    0.63f), glm::vec3(0.316228f, 0.316228f, 0.316228f), 128.0f *      0.1f }, // jade
	Material{ glm::vec3(0.05375f,    0.05f, 0.06625f), glm::vec3(0.18275f,    0.17f, 0.22525f), glm::vec3(0.332741f, 0.328634f, 0.346435f), 128.0f *      0.3f }, // obsidian
	Material{ glm::vec3( 0.1745f, 0.01175f, 0.01175f), glm::vec3(0.61424f, 0.04136f, 0.04136f), glm::vec3(0.727811f, 0.626959f, 0.626959f), 128.0f *      0.6f }, // ruby
	Material{ glm::vec3(0.24725f,  0.1995f,  0.0745f), glm::vec3(0.75164f, 0.60648f, 0.22648f), glm::vec3(0.628281f, 0.555802f, 0.366065f), 128.0f *      0.4f }, // gold
	Material{ glm::vec3(0.19225f, 0.19225f, 0.19225f), glm::vec3(0.50754f, 0.50754f, 0.50754f), glm::vec3(0.508273f, 0.508273f, 0.508273f), 128.0f *      0.4f }, // silver
	Material{ glm::vec3( 0.2125f,  0.1275f,   0.054f), glm::vec3(  0.714f,  0.4284f, 0.18144f), glm::vec3(0.393548f, 0.271906f, 0.166721f), 128.0f *      0.2f }, // bronze
	Material{ glm::vec3(    0.0f,     0.0f,     0.0f), glm::vec3(    0.5f,     0.0f,     0.0f), glm::vec3(     0.7f,      0.6f,      0.6f), 128.0f *     0.25f }, // red plastic
	Material{ glm::vec3(   0.05f,    0.05f,     0.0f), glm::vec3(    0.5f,     0.5f,     0.4f), glm::vec3(     0.7f,      0.7f,     0.04f), 128.0f * 0.078125f }, // yellow rubber
	Material{ glm::vec3(    0.1f, 0.18725f,  0.1745f), glm::vec3(  0.396f, 0.74151f, 0.69102f), glm::vec3(0.297254f,  0.30829f, 0.306678f), 128.0f *      0.1f }, // turquoise
};

const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;

// camera
Camera camera(glm::vec3(4.0f, 0.0f, 12.0f));
float lastX = SCREEN_WIDTH  / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// light / lamp
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos   = glm::vec3(0.0f, 0.0f, 2.0f);

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
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPos(window, lastX, lastY);

	// Capture the mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // GLFW_CURSOR_NORMAL - GLFW_CURSOR_DISABLED

	// Glad load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	// build and compile our shader program
	Shader cubeShader("shaders/light1.vs", "shaders/cube-with-light-material.fs");
	Shader lampShader("shaders/light1.vs", "shaders/lamp-material.fs");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float cubeVertices[] = {
		// positions         // normals
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	float lampVertices[] = {
		// base
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		// base
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		// up
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.0f,  0.5f,  0.0f,
		// up
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.0f,  0.5f,  0.0f,
		// up
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.0f,  0.5f,  0.0f,
		// up
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.0f,  0.5f,  0.0f,
	};

	VertexArray cubeVAO;
	VertexBuffer cubeVBO(cubeVertices, sizeof(cubeVertices));
	VertexBufferLayout cubeLayout;
	cubeLayout.Push<float>(3); // position - layout 0
	cubeLayout.Push<float>(3); // normals  - layout 1
	cubeVAO.AddBuffer(cubeVBO, cubeLayout);

	VertexArray lampVAO;
	VertexBuffer lampVBO(lampVertices, sizeof(lampVertices));
	VertexBufferLayout lampLayout;
	lampLayout.Push<float>(3); // position - layout 0
	lampVAO.AddBuffer(lampVBO, lampLayout);

	const unsigned int materialsLength = std::size(materials);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		// per-frame time logic
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Init GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::SliderFloat3("View Pos", &camera.Position.x, -5.0f, 5.0f);

		// Light properties
		lightPos = glm::vec3((float)materialsLength * (sin(time) * 0.5f + 0.5f), 0.0f, 2.0f);
		lightColor = glm::vec3(sin(time * 2.0f), sin(time * 0.7f), sin(time * 1.3f));
		glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
		ImGui::SliderFloat3("Light Pos", &lightPos.x, -5.0f, 5.0f);
		ImGui::ColorEdit3("Light Ambient", (float*)&ambientColor);
		ImGui::ColorEdit3("Light Diffuse", (float*)&diffuseColor);
		ImGui::ColorEdit3("Light Specular", (float*)&specularColor);

		cubeShader.use();
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.getViewMatrix();
		cubeShader.setMat4("uProjection", projectionMatrix);
		cubeShader.setMat4("uView", viewMatrix);
		cubeShader.setVec3("uViewPos", camera.Position);

		cubeShader.setVec3("uLight.position", lightPos);
		// The materials properties array considers light properties to vec3(1.0f), full intensity
		// The calculated light properties (ambientColor, diffuseColor and specularColor) should be used without the materials array
		cubeShader.setVec3("uLight.ambient",  1.0f, 1.0f, 1.0f);  // ambientColor
		cubeShader.setVec3("uLight.diffuse",  1.0f, 1.0f, 1.0f);  // diffuseColor
		cubeShader.setVec3("uLight.specular", 1.0f, 1.0f, 1.0f);  // specularColor

		glm::mat4 modelMatrix;
				
		for (unsigned int i = 0; i < materialsLength; i++)
		{
			Material mat = materials[i];
			cubeShader.setVec3("uMaterial.ambient",    mat.ambient);   // 1.0f, 0.5f, 0.31f
			cubeShader.setVec3("uMaterial.diffuse",    mat.diffuse);   // 1.0f, 0.5f, 0.31f
			cubeShader.setVec3("uMaterial.specular",   mat.specular);  // 0.5f, 0.5f, 0.5f
			cubeShader.setFloat("uMaterial.shininess", mat.shininess); // 32.0f

			if (ImGui::CollapsingHeader(("Material " + std::to_string(i)).c_str()))
			{
				ImGui::ColorEdit3("Material Ambient", (float*)&mat.ambient);
				ImGui::ColorEdit3("Material Diffuse", (float*)&mat.diffuse);
				ImGui::ColorEdit3("Material Specular", (float*)&mat.specular);
			}

			modelMatrix = glm::mat4(1.0f);
			glm::vec3 pos = glm::vec3(i * 1.1f, 0.0f, 0.0f);
			modelMatrix = glm::translate(modelMatrix, pos);
			cubeShader.setMat4("uModel", modelMatrix);

			// render the cube
			cubeVAO.Bind();
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// now render the lamp
		lampShader.use();
		lampShader.setMat4("uProjection", projectionMatrix);
		lampShader.setMat4("uView", viewMatrix);
		lampShader.setVec3("uLight.ambient", ambientColor);
		lampShader.setVec3("uLight.diffuse", diffuseColor);
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, lightPos);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		lampShader.setMat4("uModel", modelMatrix);
		lampVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 18);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window); // Swap the front buffer and the back buffer
		glfwPollEvents(); // Checks if any events are triggered (like keyboard)
	}

	// De-allocate all resources once they were already used
	cubeVAO.Delete();
	cubeVBO.Delete();
	lampVAO.Delete();
	lampVBO.Delete();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	// close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// enable 'wireframe' mode
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// disable 'wireframe' mode
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.processMouseScroll(yoffset);
}
