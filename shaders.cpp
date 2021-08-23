#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"uniform vec4 ourColor; // we set this variable in the OpenGL code.\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"};\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Enable wireframe drawing

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Disable wireframe drawing
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		wWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// Glfw window creation
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << '\n';
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Glad load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << '\n';
		return -1;
	}

	// Call function framebuffer_size_callback whenever the window changes in size
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

	// Shader program - Link vertex and fragment shader into one object
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Check if linking failed
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
	}

	// Once the linking is done, we don't need the shader objects anymore
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex input data
	float vertices[] = {
		// positions
		0.5f, -0.5f, 0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f   // top
	};

	unsigned int VBO; // Vertex Buffer Object. Used to store the vertices in the GPU's memory
	unsigned int VAO; // Vertex Array Object. Used to make it easy to switch between vertex buffers / vertex attributes
	
	glGenVertexArrays(1, &VAO); // Generate vertex array ID
	glGenBuffers(1, &VBO); // Generate vertex buffer ID

	// Setup first triangle
	glBindVertexArray(VAO); // Bind the vertex array before the vertex buffer(s)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Vertex objects has buffer type GL_ARRAY_BUFFER
	// Copy user data (vertices) into the currently bound buffer (VBO wich was binded to GL_ARRAY_BUFFER)
	// Now we have vertex data stored in the GPU memory managed by a vertex buffer object VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.9f, 1.0f); // Set the color used to clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram); // Set what shader program the render calls will use

		float time = glfwGetTime();
		float green = sin(time) / 2.0f + 0.5f;
		int vertexLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexLocation, 0.0f, green, 0.0f, 1.0f);

		glDrawArrays(
			GL_TRIANGLES, // Mode we want to draw
			0, // Start index of the vertex array we want to draw
			3 // How many vertices we want to draw
		);

		glfwSwapBuffers(window); // Swap the front buffer and the back buffer
		glfwPollEvents(); // Checks if any events are triggered (like keyboard)
	}

	// De-allocate all resources once they were already used
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}