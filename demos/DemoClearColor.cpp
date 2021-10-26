#include "DemoClearColor.h"

#include <glad/glad.h>
#include <imgui/imgui.h>

namespace demo {

	DemoClearColor::DemoClearColor() : m_ClearColor { 0.3f, 0.5f, 0.8f }
	{

	}

	DemoClearColor::~DemoClearColor()
	{

	}

	void DemoClearColor::OnUpdate(float deltaTime)
	{

	}

	void DemoClearColor::OnRender()
	{
		glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void DemoClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit3("Background Color", (float*)&m_ClearColor);
	}

}