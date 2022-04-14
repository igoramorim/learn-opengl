#pragma once

#include "Constants.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	// WindowManager();
	WindowManager(WindowManager const&) = delete;
	WindowManager& operator=(WindowManager const&) = delete;

	~WindowManager();

	static std::shared_ptr<WindowManager> instance()
	{
		static std::shared_ptr<WindowManager> s{ new WindowManager };
		return s;
	}

	GLFWwindow* GetWindow();
	bool WindowShouldClose();
	void SwapBuffers();
	void PollEvents();
	void Terminate();
	void SetInputMode(int mode = GLFW_CURSOR, int value = GLFW_CURSOR_NORMAL);
	void SetCursorPos(float x = Constants::LAST_X, float y = Constants::LAST_Y);
	void CalculateDeltaTime();
	float GetDeltaTime();
	void ClearScreen(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

private:
	WindowManager();

	GLFWwindow* m_Window;
	float m_DeltaTime; // time between current frame and last frame
	float m_LastFrame;
};
