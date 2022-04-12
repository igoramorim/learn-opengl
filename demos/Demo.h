#pragma once

#include "../Constants.h"

#include <iostream>
#include <functional> // std::function
#include <vector>
#include <string>

// TODO: I don't think these includes are correct
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace demo {

	class Demo
	{
	public:
		Demo() {}

		// De-allocate all resources once they were already used
		virtual ~Demo() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
		virtual void ProcessInput(GLFWwindow* window) {}

	private:
		virtual void BindGlfwFunctions(GLFWwindow* window) {}

	protected:
		float m_DeltaTime;
		float m_LastX = Constants::LAST_X;
		float m_LastY = Constants::LAST_Y;

	};

	class DemoMenu : public Demo
	{
	public:
		DemoMenu(Demo*& currentDemoPointer);
		void OnImGuiRender() override;

		template<typename T>
		void RegisterDemo(const std::string& name)
		{
			std::cout << "Registering demo: " << name << '\n';
			m_Demos.push_back(std::make_pair(name, []() { return new T(); }));
		}

		void ProcessInput(GLFWwindow* window)
		{
			// close window
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}

	private:
		Demo*& m_CurrentDemo;
		std::vector<std::pair<std::string, std::function<Demo* ()>>> m_Demos;
	};

}