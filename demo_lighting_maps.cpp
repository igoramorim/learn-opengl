#include <iostream>

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
#include "shader.h"
#include "texture.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// settings
const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;
glm::vec3 backgroundColor = glm::vec3(0.2f, 0.2f, 0.2f);

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCREEN_WIDTH  / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// light / lamp
glm::vec3 lightPos           = glm::vec3(0.0f, 0.0f, 2.0f);
glm::vec3 lightDiffuseColor  = glm::vec3(0.5f, 0.5f, 0.5f);
glm::vec3 lightAmbientColor  = glm::vec3(0.2f, 0.2f, 0.2f);
glm::vec3 lightSpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);

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
	Shader cubeShader("shaders/lighting-maps.vs", "shaders/lighting-maps.fs");
	Shader lampShader("shaders/light1.vs", "shaders/lamp-material.fs");

	Texture diffuseMapTex("textures/container2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture specularMapTex("textures/container2_specular.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture emissionMapTex("textures/container2_emission.png", GL_TEXTURE_2D, GL_TEXTURE2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE);
		
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	float cubeVertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
	cubeLayout.Push<float>(3); // position  - layout 0
	cubeLayout.Push<float>(3); // normals   - layout 1
	cubeLayout.Push<float>(2); // texCoords - layout 2
	cubeVAO.AddBuffer(cubeVBO, cubeLayout);

	VertexArray lampVAO;
	VertexBuffer lampVBO(lampVertices, sizeof(lampVertices));
	VertexBufferLayout lampLayout;
	lampLayout.Push<float>(3); // position - layout 0
	lampVAO.AddBuffer(lampVBO, lampLayout);

	diffuseMapTex.texUnit(cubeShader, "uMaterial.diffuse", 0);
	specularMapTex.texUnit(cubeShader, "uMaterial.specular", 1);
	emissionMapTex.texUnit(cubeShader, "uMaterial.emission", 2);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		// per-frame time logic
		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Init GUI
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");
		ImGui::ColorEdit3("Background Color", (float*)&backgroundColor);
		ImGui::SliderFloat3("View Pos", &camera.Position.x, -5.0f, 5.0f);

		// Light properties
		ImGui::SliderFloat3("Light Pos", &lightPos.x, -5.0f, 5.0f);
		ImGui::ColorEdit3("Light Ambient", (float*)&lightAmbientColor);
		ImGui::ColorEdit3("Light Diffuse", (float*)&lightDiffuseColor);
		ImGui::ColorEdit3("Light Specular", (float*)&lightSpecularColor);

		cubeShader.use();
		// Matrix
		glm::mat4 projectionMatrix = glm::perspective(glm::radians(camera.Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewMatrix = camera.getViewMatrix();
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		
		// Uniform matrix / positions
		cubeShader.setMat4("uProjection", projectionMatrix);
		cubeShader.setMat4("uView", viewMatrix);
		cubeShader.setVec3("uViewPos", camera.Position);
		cubeShader.setMat4("uModel", modelMatrix);

		// Uniform light properties
		cubeShader.setVec3("uLight.position", lightPos);
		cubeShader.setVec3("uLight.ambient", lightAmbientColor);
		cubeShader.setVec3("uLight.diffuse", lightDiffuseColor);
		cubeShader.setVec3("uLight.specular", lightSpecularColor);

		// Uniform material properties
		cubeShader.setFloat("uMaterial.shininess", 64.0f);

		// Other stuff
		cubeShader.setFloat("uTime", time);
		
		// Activate and bind lighting maps
		diffuseMapTex.activeAndBind();
		specularMapTex.activeAndBind();
		emissionMapTex.activeAndBind();
		
		// render the cube
		cubeVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// now render the lamp
		lampShader.use();
		lampShader.setMat4("uProjection", projectionMatrix);
		lampShader.setMat4("uView", viewMatrix);
		lampShader.setVec3("uLight.ambient", lightAmbientColor);
		lampShader.setVec3("uLight.diffuse", lightDiffuseColor);
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
