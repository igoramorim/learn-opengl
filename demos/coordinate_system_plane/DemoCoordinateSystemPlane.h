#pragma once

#include "../Demo.h"
#include "../../shader.h"
#include "../../texture.h"

namespace demo {

	class DemoCoordinateSystemPlane : public Demo
	{
	public:
		DemoCoordinateSystemPlane();
		~DemoCoordinateSystemPlane();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		unsigned int m_VBO;
		unsigned int m_VAO;
		unsigned int m_EBO;
		Texture m_WoodTex;
		Texture m_SmileTex;
		glm::mat4 m_ModelMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
	};

}