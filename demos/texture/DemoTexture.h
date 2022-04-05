#pragma once

#include "../Demo.h"
#include "../../shader.h"

namespace demo {

	class DemoTexture : public Demo
	{
	public:
		DemoTexture();
		~DemoTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		unsigned int m_VBO;
		unsigned int m_VAO;
		unsigned int m_EBO;
		unsigned int m_Texture1;
		unsigned int m_Texture2;
	};

}