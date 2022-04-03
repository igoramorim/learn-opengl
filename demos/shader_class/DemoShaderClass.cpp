#include "DemoShaderClass.h"

namespace demo {

	DemoShaderClass::DemoShaderClass() :
		m_Shader{ "shaders/shader1.vs", "shaders/shader1.fs" } // build and compile our shader program
	{
		// Vertex input data
		float vertices[] = {
			// positions        // colors
			1.0f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		   -1.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
			0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
		};

		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);

		// Setup triangle
		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(
			1, // Wich vertex we want to configure. Vertex shader location = 1
			3, // Size of the vertex attribute. Our input data is a vec3, so it is composed of 3 values
			GL_FLOAT, // Type of the data. vec3 is a vector of 3 floats
			GL_FALSE, // Data should be normalized?
			6 * sizeof(float), // Stride: Space between consecutive vertex attributes. Our input data has 9 float values, wich will be 3 packs of floats
			(void*)(3 * sizeof(float)) // Offset where the position data begins in the buffer
		);
		glEnableVertexAttribArray(1); // Enable attribute ID 0 wich was set in glVertexAttribPointer
	}

	DemoShaderClass::~DemoShaderClass()
	{
		glDeleteVertexArrays(1, &m_VAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void DemoShaderClass::OnUpdate(float deltaTime)
	{
		m_Time = deltaTime;
	}

	void DemoShaderClass::OnRender()
	{
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		m_Shader.use();
		m_Shader.setFloat("time", m_Time);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void DemoShaderClass::OnImGuiRender()
	{

	}

}