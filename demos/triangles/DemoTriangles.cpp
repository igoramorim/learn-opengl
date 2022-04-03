#include "DemoTriangles.h"

#include <glad/glad.h>
#include <imgui/imgui.h>

namespace demo {

	DemoTriangles::DemoTriangles()
	{
		const char* vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"out vec4 vertexColor; // specify a color output to the fragment shader\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
			"   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
			"}\0";

		const char* fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec4 vertexColor; // the input variable from the vertex shader (same name and same type)\n"
			"void main()\n"
			"{\n"
			"   //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
			"   FragColor = vertexColor;\n"
			"};\0";

		const char* fragmentShaderSource2 = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"void main()\n"
			"{\n"
			"	FragColor = vec4(0.2f, 0.9f, 1.0f, 1.0f);\n"
			"};\0";

		// Vertex shader
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Int because it is the ID of the shader object
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the shader source code to the shader object
		glCompileShader(vertexShader);
		// Check if vertex shader compiled successful
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		}

		// Fragment shader
		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		// Check if fragment shader compiled successful
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		}

		// Fragment shader 2
		unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
		glCompileShader(fragmentShader2);
		// Check if fragment shader compiled successful
		glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		}

		// Shader program - Link vertex and fragment shader into one object
		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, vertexShader);
		glAttachShader(m_ShaderProgram, fragmentShader);
		glLinkProgram(m_ShaderProgram);
		// Check if linking failed
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		}

		// Shader program 2
		m_ShaderProgram2 = glCreateProgram();
		glAttachShader(m_ShaderProgram2, vertexShader);
		glAttachShader(m_ShaderProgram2, fragmentShader2);
		glLinkProgram(m_ShaderProgram2);
		// Check if linking failed
		glGetProgramiv(m_ShaderProgram2, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(m_ShaderProgram2, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		}
		// Once the linking is done, we don't need the shader objects anymore
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteShader(fragmentShader2);

		// Vertex input data
		float firstTriangle[] = {
			-0.9f, -0.5f, 0.0f,  // left 
			-0.0f, -0.5f, 0.0f,  // right
			-0.45f, 0.5f, 0.0f,  // top 
		};

		float secondTriangle[] = {
			0.0f, -0.5f, 0.0f,  // left
			0.9f, -0.5f, 0.0f,  // right
			0.45f, 0.5f, 0.0f   // top 
		};

		// Vertex Array Object. Used to make it easy to switch between vertex buffers / vertex attributes
		glGenVertexArrays(2, m_VAOs); // Generate vertex array ID
		glGenBuffers(2, m_VBOs); // Generate vertex buffer ID

		// Setup first triangle
		glBindVertexArray(m_VAOs[0]); // Bind the vertex array before the vertex buffer(s)
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[0]); // Vertex objects has buffer type GL_ARRAY_BUFFER
		// Copy user data (vertices) into the currently bound buffer (VBO wich was binded to GL_ARRAY_BUFFER)
		// Now we have vertex data stored in the GPU memory managed by a vertex buffer object VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
		// We need to tell OpenGL how to read the vertex input data
		glVertexAttribPointer(
			0, // Wich vertex we want to configure. Vertex shader location = 0
			3, // Size of the vertex attribute. Our input data is a vec3, so it is composed of 3 values
			GL_FLOAT, // Type of the data. vec3 is a vector of 3 floats
			GL_FALSE, // Data should be normalized?
			3 * sizeof(float), // Stride: Space between consecutive vertex attributes. Our input data has 9 float values, wich will be 3 packs of floats
			(void*)0 // Offset where the position data begins in the buffer
		);
		glEnableVertexAttribArray(0); // Enable attribute ID 0 wich was set in glVertexAttribPointer

		// Setup second triangle
		glBindVertexArray(m_VAOs[1]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
	}

	DemoTriangles::~DemoTriangles()
	{
		// De-allocate all resources once they were already used
		glDeleteVertexArrays(2, m_VAOs);
		glDeleteBuffers(2, m_VBOs);
		glDeleteProgram(m_ShaderProgram);
		glDeleteProgram(m_ShaderProgram2);
	}

	void DemoTriangles::OnUpdate(float deltaTime)
	{

	}

	void DemoTriangles::OnRender()
	{
		glClearColor(0.2f, 0.3f, 0.9f, 1.0f); // Set the color used to clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_ShaderProgram); // Set what shader program the render calls will use
		glBindVertexArray(m_VAOs[0]); // Draw first triangle using the data from the first VAO
		glDrawArrays(
			GL_TRIANGLES, // Mode we want to draw
			0, // Start index of the vertex array we want to draw
			3 // How many vertices we want to draw
		);

		glUseProgram(m_ShaderProgram2);
		glBindVertexArray(m_VAOs[1]); // Draw the second triangle using the data from the second VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void DemoTriangles::OnImGuiRender()
	{

	}

}