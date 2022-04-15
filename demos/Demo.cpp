#include "Demo.h"
#include "../WindowManager.h"

#include <imgui/imgui.h>

namespace demo {

	Demo::Demo() :
		m_Window{ WindowManager::instance()->GetWindow() },
		m_DeltaTime{ 0.0f }
	{
		SetGlfwCallbackFunctions();
	}

	void Demo::SetGlfwCallbackFunctions()
	{
		std::cout << "Demo: Setting GLFW callbacks\n";

		glfwSetWindowUserPointer(m_Window, this);

		auto funcSetFrameBufferSize = [](GLFWwindow* window, int width, int height)
		{
			static_cast<Demo*>(glfwGetWindowUserPointer(window))->FramebufferSizeCallback(window, width, height);
		};

		glfwSetFramebufferSizeCallback(m_Window, funcSetFrameBufferSize);
	}

	void Demo::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Demo::ProcessInput()
	{
		// close window
		if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_Window, true);
	}

	/*
	* ===================================================================================================
	*/

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