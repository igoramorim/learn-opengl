#include "DemoTransformations.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace demo {

	DemoTransformations::DemoTransformations() :
		m_Shader{ "shaders/transformation.vs", "shaders/transformation.fs" },
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
	}

	DemoTransformations::~DemoTransformations()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		m_WoodTex.Delete();
		m_SmileTex.Delete();
	}

	void DemoTransformations::OnUpdate(float deltaTime)
	{
		m_Time = deltaTime;
	}

	void DemoTransformations::OnRender()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_WoodTex.activeAndBind();
		m_SmileTex.activeAndBind();

		m_Shader.use();

		/* Transformations
		* ===================================================================================
		* The matrix multiplication is applied in reverse (from bottom to top). So the order is:
		* 1. Scale
		* 2. Rotate
		* 3. Translate
		* Try swtiching the order between translate and rotate. When the translate is applied first what happens is:
		* Its rotation origin is no longer (0,0,0) making it looks as if its circling around the origin of the scene
		*/
		// identity matrix
		// 1 0 0 0
		// 0 1 0 0
		// 0 0 1 0
		// 0 0 0 1

		/* Transformation 1
		* ===================================================================================
		*/
		glm::mat4 transform = glm::mat4(1.0f); // identity matrix
		transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
		transform = glm::rotate(transform, m_Time, glm::vec3(0.0f, 0.0f, 1.0f));
		m_Shader.setMat4("transform", transform);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ==================================================================================

		/* Transformation 2
		* ===================================================================================
		*/
		transform = glm::mat4(1.0f); // reset to identity matrix
		transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f)); // (3)
		transform = glm::rotate(transform, m_Time, glm::vec3(0.0f, 0.0f, 1.0f)); // (2)
		float s = sin(m_Time) * 0.5 + 0.5;
		transform = glm::scale(transform, glm::vec3(s, s, 1.0f)); // (1)
		m_Shader.setMat4("transform", transform);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// ==================================================================================

	}

	void DemoTransformations::OnImGuiRender()
	{
	}

}