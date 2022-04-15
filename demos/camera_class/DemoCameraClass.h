#pragma once

#include "../Demo.h"
#include "../../shader.h"
#include "../../texture.h"
#include "../../camera.h"

namespace demo {

	class DemoCameraClass : public Demo
	{
	public:
		DemoCameraClass();
		~DemoCameraClass();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInput() override;
		
	private:
		unsigned int m_VBO;
		unsigned int m_VAO;
		Shader m_Shader;
		Texture m_WoodTex;
		Texture m_SmileTex;
		Camera m_Camera;
		glm::vec3 m_CubePositions[5];

		void SetGlfwCallbackFunctions() override;
		void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	};
}
