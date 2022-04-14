#include "DemoCameraClass.h"
#include "../../Constants.h"
#include "../../WindowManager.h";

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace demo {
	
	DemoCameraClass::DemoCameraClass() :
		m_Shader{ "shaders/camera.vs", "shaders/camera.fs" },
		m_WoodTex{ "textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
		m_SmileTex{ "textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
		m_Camera{ glm::vec3(0.0f, 0.0f, 3.0f) },
		m_CubePositions{
			glm::vec3( 0.0f,  0.0f,   0.0f),
			glm::vec3( 2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f,  -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3( 2.4f, -0.4f,  -3.5f)
		},
		m_Window{ WindowManager::instance()->GetWindow() }
	{
		BindGlfwFunctions();

		glEnable(GL_DEPTH_TEST);

		// Vertex input data
		float vertices[] = {
			// positions          // text coord
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		// Setup triangle
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture Coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);

		m_WoodTex.texUnit(m_Shader, "texture1", 0);
		m_SmileTex.texUnit(m_Shader, "texture2", 1);

		m_Shader.use();
	}

	DemoCameraClass::~DemoCameraClass()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		m_SmileTex.Delete();
		m_WoodTex.Delete();
	}

	void DemoCameraClass::OnUpdate(float deltaTime)
	{
		m_DeltaTime = deltaTime;
	}

	void DemoCameraClass::OnRender()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_WoodTex.activeAndBind();
		m_SmileTex.activeAndBind();

		/* Transformations to make it 3D
		* ==========================================================================
		*/
		glm::mat4 viewMatrix;
		viewMatrix = m_Camera.getViewMatrix();
		// viewMatrix = m_Camera.getViewMatrixCustomCalculation(); // example on how to calculate the view matrix manually
		m_Shader.setMat4("view", viewMatrix);

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_Camera.Fov), (float)Constants::SCREEN_WIDTH / (float)Constants::SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("projection", projectionMatrix);

		for (unsigned int i = 0; i < 5; i++)
		{
			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, m_CubePositions[i]);
			m_Shader.setMat4("model", modelMatrix);

			glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices to make a cube
		}
	}

	void DemoCameraClass::OnImGuiRender()
	{

	}

	void DemoCameraClass::ProcessInput()
	{
		// close window
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);

		// enable 'wireframe' mode
		if (glfwGetKey(m_Window, GLFW_KEY_L) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// disable 'wireframe' mode
		if (glfwGetKey(m_Window, GLFW_KEY_F) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		// camera movement
		if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera.processKeyboard(FORWARD, m_DeltaTime);
		if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera.processKeyboard(BACKWARD, m_DeltaTime);
		if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera.processKeyboard(LEFT, m_DeltaTime);
		if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera.processKeyboard(RIGHT, m_DeltaTime);
	}

	void DemoCameraClass::BindGlfwFunctions()
	{
		std::cout << "Bind function : DemoraCameraClass\n";
		// glfwSetCursorPosCallback(window, mouse_callback);
		// glfwSetScrollCallback(window, scroll_callback);
	}

	/*void DemoCameraClass::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		float xoffset = xpos - m_LastX;
		float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

		m_LastX = xpos;
		m_LastY = ypos;

		m_Camera.processMouseMovement(xoffset, yoffset);
	}

	void DemoCameraClass::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_Camera.processMouseScroll(yoffset);
	}*/

}