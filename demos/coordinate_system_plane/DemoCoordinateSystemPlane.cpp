#include "DemoCoordinateSystemPlane.h"
#include "../../Constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace demo {

	DemoCoordinateSystemPlane::DemoCoordinateSystemPlane() :
		m_Shader{ "shaders/coordinate-system.vs", "shaders/coordinate-system.fs" },
		m_WoodTex{ "textures/container.jpg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE },
		m_SmileTex{ "textures/awesomeface.png", GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE }
	{
		// Vertex input data
		float vertices[] = {
			// positions         // colors           // texture coords
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
		};

		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		// Setup triangle
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// Texture Coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);

		m_WoodTex.texUnit(m_Shader, "texture1", 0);
		m_SmileTex.texUnit(m_Shader, "texture2", 1);

		m_Shader.use();

		/* Transformations to make it 3D
		* ==========================================================================
		*/
		// model matrix applies transformations to the object's vertices 
		m_ModelMatrix = glm::mat4(1.0f); // start with an identity matrix
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // rotate 55 degrees in x-axis

		// view matrix applies transformations to the 'eye' but actually we move the entire scene
		// so if you want to move the camera backwards, you move the entire scene forward
		m_ViewMatrix = glm::mat4(1.0f);
		// note that we're translating the scene in the reverse direction of where we want to move
		m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(0.0f, 0.0f, -2.0f));

		// projection matrix
		m_ProjectionMatrix = glm::perspective(
			glm::radians(45.0f),						// fov - field of view
			(float)Constants::SCREEN_WIDTH / (float)Constants::SCREEN_HEIGHT,	// aspect ratio
			0.1f,										// near plane - vertices between the near plane and the 'eye' wont be rendered
			100.0f);									// far plane - vertices after it wont be rendered
		// =========================================================================
	}

	DemoCoordinateSystemPlane::~DemoCoordinateSystemPlane()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		m_WoodTex.Delete();
		m_SmileTex.Delete();
	}

	void DemoCoordinateSystemPlane::OnUpdate(float deltaTime)
	{

	}

	void DemoCoordinateSystemPlane::OnRender()
	{
		glClearColor(0.3f, 0.8f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_WoodTex.activeAndBind();
		m_SmileTex.activeAndBind();

		m_Shader.setMat4("model", m_ModelMatrix);
		m_Shader.setMat4("view", m_ViewMatrix);
		m_Shader.setMat4("projection", m_ProjectionMatrix);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void DemoCoordinateSystemPlane::OnImGuiRender()
	{

	}

}