#include "DemoCoordinateSystemCube.h"
#include "../../Constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace demo {

	DemoCoordinateSystemCube::DemoCoordinateSystemCube() :
		m_Shader{ "shaders/coordinate-system.vs", "shaders/coordinate-system.fs" },
		m_WoodTex{ "textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
		m_SmileTex{ "textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE },
		m_ModelMatrix{ glm::mat4(1.0f) },
		m_ViewMatrix{ glm::mat4(1.0f) },
		m_ProjectionMatrix{ glm::mat4(1.0f) }
	{
		// Vertex input data
		float vertices[] = {
			// positions          // text coord
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		// Setup triangle
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Texture Coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);

		m_WoodTex.texUnit(m_Shader, "texture1", 0);
		m_SmileTex.texUnit(m_Shader, "texture2", 1);

		m_Shader.use();

		/*
		* Enable Depth Test - OpenGL stores all depth information in the z-buffer (depth buffer)
		* the depth is stored within each fragment. When the fragment want to output a color, OpenGL compares its depth value with the z-buffer
		* if the current fragment is behind the other fragment it is discarded, otherwise overwritten
		* GL_DEPTH_BUFFER_BIT is also needed in the glClear to make it work
		*
		* Comment the glEnable below and see that some faces of the cube are above others
		* that happens because OpenGL does not do the depth test, and since OpenGL does not guarantee the order the triangles
		* are rendered, some triangles are drawn on top of each other
		*/
		glEnable(GL_DEPTH_TEST);
	}

	DemoCoordinateSystemCube::~DemoCoordinateSystemCube()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		m_WoodTex.Delete();
		m_SmileTex.Delete();
	}

	void DemoCoordinateSystemCube::OnUpdate(float deltaTime)
	{
		m_Time = deltaTime;
	}

	void DemoCoordinateSystemCube::OnRender()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_WoodTex.activeAndBind();
		m_SmileTex.activeAndBind();

		/* Transformations to make it 3D
		* ==========================================================================
		*/
		m_ModelMatrix = glm::mat4(1.0f);
		m_ViewMatrix = glm::mat4(1.0f);
		m_ProjectionMatrix = glm::mat4(1.0f);

		m_ModelMatrix = glm::rotate(m_ModelMatrix, m_Time * glm::radians(50.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
		m_ProjectionMatrix = glm::perspective(glm::radians(45.0f), (float)Constants::SCREEN_WIDTH / (float)Constants::SCREEN_HEIGHT, 0.1f, 100.0f);

		m_Shader.setMat4("model", m_ModelMatrix);
		m_Shader.setMat4("view", m_ViewMatrix);
		m_Shader.setMat4("projection", m_ProjectionMatrix);
		// =========================================================================

		glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices to make a cube
	}

	void DemoCoordinateSystemCube::OnImGuiRender()
	{

	}

}