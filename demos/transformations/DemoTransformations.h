#pragma once

#include "../Demo.h"
#include "../../shader.h"
#include "../../texture.h"

namespace demo {

	class DemoTransformations : public Demo
	{
	public:
		DemoTransformations();
		~DemoTransformations();

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
	};

}
