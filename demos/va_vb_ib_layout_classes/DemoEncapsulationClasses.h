#pragma once

#include "../Demo.h"
#include "../../shader.h"
#include "../../texture.h"
#include "../../camera.h"
#include "../../VertexBuffer.h";
#include "../../VertexArray.h";
#include "../../IndexBuffer.h";

namespace demo {

	class DemoEncapsulationClasses : public Demo
	{
	public:
		DemoEncapsulationClasses();
		~DemoEncapsulationClasses();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInput() override;

	private:
		Shader m_Shader;
		Texture m_WoodTex;
		Texture m_SmileTex;
		Camera m_Camera;
		VertexArray m_VA;
		// VertexBuffer m_VB;
		IndexBuffer m_IB;
		VertexBufferLayout m_VBLayout;
		float m_Vertex[40];
		unsigned int m_Indices[36];

		void SetGlfwCallbackFunctions() override;
		void MouseCallback(GLFWwindow* window, double xpos, double ypos);
		void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	};

}
