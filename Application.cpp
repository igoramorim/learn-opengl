#include <iostream>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "WindowManager.h"
#include "Constants.h"

#include "camera.h"
#include "demos/clear_color/DemoClearColor.h"
#include "demos/triangles/DemoTriangles.h"
#include "demos/shader_uniform/DemoShaderUniform.h"
#include "demos/shader_class/DemoShaderClass.h"
#include "demos/texture/DemoTexture.h"
#include "demos/texture_class/DemoTextureClass.h"
#include "demos/transformations/DemoTransformations.h"
#include "demos/coordinate_system_plane/DemoCoordinateSystemPlane.h"
#include "demos/coordinate_system_cube/DemoCoordinateSystemCube.h"
#include "demos/camera_class/DemoCameraClass.h"
#include "demos/va_vb_ib_layout_classes/DemoEncapsulationClasses.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

int main()
{
	auto windowManager = WindowManager::instance();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowManager->GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	demo::Demo* currentDemo = nullptr;
	demo::DemoMenu* menu = new demo::DemoMenu(currentDemo);
	currentDemo = menu;

	menu->RegisterDemo<demo::DemoClearColor>("Clear Color");
	menu->RegisterDemo<demo::DemoTriangles>("Triangles");
	menu->RegisterDemo<demo::DemoShaderUniform>("Shader Uniform");
	menu->RegisterDemo<demo::DemoShaderClass>("Shader Class");
	menu->RegisterDemo<demo::DemoTexture>("Texture");
	menu->RegisterDemo<demo::DemoTextureClass>("Texture Class");
	menu->RegisterDemo<demo::DemoTransformations>("Transformations");
	menu->RegisterDemo<demo::DemoCoordinateSystemPlane>("Coordinate System: Plane");
	menu->RegisterDemo<demo::DemoCoordinateSystemCube>("Coordinate System: Cube");
	menu->RegisterDemo<demo::DemoCameraClass>("Camera Class");
	menu->RegisterDemo<demo::DemoEncapsulationClasses>("Encapsulation Classes");

	// Render loop
	while (!windowManager->WindowShouldClose())
	{
		windowManager->CalculateDeltaTime();

		windowManager->ClearScreen();

		// Init ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Debug");

		if (currentDemo)
		{
			currentDemo->ProcessInput();
			currentDemo->OnUpdate(windowManager->GetDeltaTime());
			currentDemo->OnRender();
			if (currentDemo != menu && ImGui::Button("Back"))
			{
				delete currentDemo;
				currentDemo = menu;
			}
			currentDemo->OnImGuiRender();
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("General settings");
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("ImGui %s", IMGUI_VERSION);
		ImGui::End();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		windowManager->SwapBuffers();
		windowManager->PollEvents();
	}

	if (currentDemo != menu)
		delete menu;
	else
		delete currentDemo;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	windowManager->Terminate();
	return 0;
}
