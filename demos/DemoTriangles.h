#pragma once

#include "Demo.h"

namespace demo {

	class DemoTriangles : public Demo
	{
	public:
		DemoTriangles();
		~DemoTriangles();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		unsigned int m_ShaderProgram;
		unsigned int m_ShaderProgram2;
		unsigned int m_VAOs[2];
	};

}