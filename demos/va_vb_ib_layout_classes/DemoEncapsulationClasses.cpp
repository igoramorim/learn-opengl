#include "DemoEncapsulationClasses.h"
#include "../../Constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

namespace demo {

	DemoEncapsulationClasses::DemoEncapsulationClasses() :
		m_Shader{ "shaders/camera.vs", "shaders/camera.fs" },
		m_WoodTex{ "textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
		m_SmileTex{ "textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
		m_Camera{ glm::vec3(0.0f, 0.0f, 3.0f) },
		m_Vertex{
			// positions          // text coord
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, //  0 bottom left  back
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, //  1 bottom right back
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, //  2 top    right back
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, //  3 top    left  back
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, //  4 bottom right front
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, //  5 top    right front
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, //  6 top    left  front
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, //  7 bottom left  front
		},
		m_Indices{
			// back
			0, 1, 2,
			0, 2, 3,
			// right
			1, 2, 4,
			2, 4, 5,
			// top
			2, 5, 6,
			2, 3, 6,
			// left
			0, 3, 7,
			3, 6, 7,
			// bottom
			0, 1, 7,
			1, 4, 7,
			// front
			4, 5, 7,
			5, 6, 7
		},
		// m_VB{ m_Vertex, sizeof(m_Vertex) },
		m_IB{ m_Indices, sizeof(m_Indices) }
	{
		SetGlfwCallbackFunctions();

		glEnable(GL_DEPTH_TEST);

		/* TODO: for some reason the triangles does not render
		*  when using the private member m_VB initialized in the constructor
		* 
		* When using the m_VB below it works
		*/
		VertexBuffer m_VB(m_Vertex, sizeof(m_Vertex));

		// IndexBuffer m_IB(m_Indices, sizeof(m_Indices));

		m_VBLayout.Push<float>(3); // position      - layout 0
		m_VBLayout.Push<float>(2); // texture coord - layout 1

		m_VA.AddBuffer(m_VB, m_VBLayout);

		m_WoodTex.texUnit(m_Shader, "texture1", 0);
		m_SmileTex.texUnit(m_Shader, "texture2", 1);

		m_Shader.use();
	}

	DemoEncapsulationClasses::~DemoEncapsulationClasses()
	{
		// m_VA.Delete();
		// m_VB.Delete();
		// m_IB.Delete();
		m_SmileTex.Delete();
		m_WoodTex.Delete();
	}

	void DemoEncapsulationClasses::OnUpdate(float deltaTime)
	{
		m_DeltaTime = deltaTime;
	}

	void DemoEncapsulationClasses::OnRender()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// m_VA.Bind();
		// m_IB.Bind();

		m_WoodTex.activeAndBind();
		m_SmileTex.activeAndBind();

		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = m_Camera.getViewMatrix();
		m_Shader.setMat4("view", viewMatrix);

		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		projectionMatrix = glm::perspective(glm::radians(m_Camera.Fov), (float)Constants::SCREEN_WIDTH / (float)Constants::SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("projection", projectionMatrix);

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		m_Shader.setMat4("model", modelMatrix);

		glDrawArrays(GL_TRIANGLES, 0, 36); // Use with VertexArrays
		// glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // Use with IndexBuffers
	}

	void DemoEncapsulationClasses::OnImGuiRender()
	{
		ImGui::SliderFloat3("Camera Pos", &m_Camera.Position.x, -5.0f, 5.0f);
	}

	void DemoEncapsulationClasses::ProcessInput()
	{
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

	void DemoEncapsulationClasses::SetGlfwCallbackFunctions()
	{
		std::cout << "DemoEncapsulationClasses: Setting GLFW callbacks\n";

		glfwSetWindowUserPointer(m_Window, this);

		auto funcSetCursor = [](GLFWwindow* window, double xpos, double ypos)
		{
			static_cast<DemoEncapsulationClasses*>(glfwGetWindowUserPointer(window))->MouseCallback(window, xpos, ypos);
		};

		auto funcSetScroll = [](GLFWwindow* window, double xoffset, double yoffset)
		{
			static_cast<DemoEncapsulationClasses*>(glfwGetWindowUserPointer(window))->ScrollCallback(window, xoffset, yoffset);
		};

		glfwSetCursorPosCallback(m_Window, funcSetCursor);
		glfwSetScrollCallback(m_Window, funcSetScroll);
	}

	void DemoEncapsulationClasses::MouseCallback(GLFWwindow* window, double xpos, double ypos)
	{
		float xoffset = xpos - m_LastX;
		float yoffset = m_LastY - ypos; // reversed since y-coordinates go from bottom to top

		m_LastX = xpos;
		m_LastY = ypos;

		m_Camera.processMouseMovement(xoffset, yoffset);
	}

	void DemoEncapsulationClasses::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		m_Camera.processMouseScroll(yoffset);
	}

}