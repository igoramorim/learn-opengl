#pragma once

#include "../Demo.h"
#include "../../shader.h"

namespace demo {

	class DemoShaderClass : public Demo
	{
	public:
		DemoShaderClass();
		~DemoShaderClass();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		unsigned int m_VBO;
		unsigned int m_VAO;
	};
}
