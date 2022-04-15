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
		GLFWwindow* m_Window;
		unsigned int m_VBO;
		unsigned int m_VAO;
		Shader m_Shader;
		Texture m_WoodTex;
		Texture m_SmileTex;
		Camera m_Camera;
		glm::vec3 m_CubePositions[5];

		void BindGlfwFunctions() override;
	};
}
