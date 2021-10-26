#include "Demo.h"

#include <imgui/imgui.h>

namespace demo {

	DemoMenu::DemoMenu(Demo*& currentDemoPointer) : m_CurrentDemo(currentDemoPointer)
	{

	}

	void DemoMenu::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("Demos"))
		{
			for (auto& demo : m_Demos)
			{
				if (ImGui::Button(demo.first.c_str()))
				{
					m_CurrentDemo = demo.second();
				}
			}
		}
	}

}