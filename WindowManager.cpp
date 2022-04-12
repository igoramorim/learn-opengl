#include <iostream>

#include "WindowManager.h"

WindowManager::WindowManager() :
	m_DeltaTime{ 0.0f },
	m_LastFrame{ 0.0f }
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	wWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Glfw window creation
	m_Window = glfwCreateWindow(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (m_Window == NULL)
	{
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		// return -1;
	}

	glfwMakeContextCurrent(m_Window);
	SetCursorPos();
	SetInputMode();

	// Glad load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		// return -1;
	}
}

GLFWwindow* WindowManager::GetWindow()
{
	return m_Window;
}

bool WindowManager::WindowShouldClose()
{
	return glfwWindowShouldClose(m_Window);
}

void WindowManager::SwapBuffers()
{
	glfwSwapBuffers(m_Window); // Swap the front buffer and the back buffer
}

void WindowManager::PollEvents()
{
	glfwPollEvents(); // Checks if any events are triggered (like keyboard)
}

void WindowManager::Terminate()
{
	glfwTerminate();
}

void WindowManager::SetInputMode(int mode, int value)
{
	// Capture the mouse cursor
	glfwSetInputMode(m_Window, mode, value); // GLFW_CURSOR_NORMAL - GLFW_CURSOR_DISABLED
}

void WindowManager::SetCursorPos(float x, float y)
{
	glfwSetCursorPos(m_Window, x, y);
}

void WindowManager::CalculateDeltaTime()
{
	float time = glfwGetTime();
	// per-frame time logic
	float currentFrame = time;
	m_DeltaTime = currentFrame - m_LastFrame;
	m_LastFrame = currentFrame;
}

float WindowManager::GetDeltaTime()
{
	return m_DeltaTime;
}

void WindowManager::ClearScreen(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

WindowManager::~WindowManager()
{
}
