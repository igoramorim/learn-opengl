#pragma once

#include <iostream>
#include <functional> // std::function
#include <vector>
#include <string>

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

	protected:
		float m_Time;
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

	private:
		Demo*& m_CurrentDemo;
		std::vector<std::pair<std::string, std::function<Demo* ()>>> m_Demos;
	};

}