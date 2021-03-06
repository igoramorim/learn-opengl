#pragma once

#include "../Demo.h"

namespace demo {

	class DemoShaderUniform : public Demo
	{
	public:
		DemoShaderUniform();
		~DemoShaderUniform();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		unsigned int m_ShaderProgram;
		unsigned int m_VBO;
		unsigned int m_VAO;
	};

}
