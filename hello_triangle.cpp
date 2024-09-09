#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

// Function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void createShaderProgram(void);

// Variables
unsigned int VBO;				// Buffer for Vertex Data
unsigned int VAO;				// Vertex Array Object
unsigned int vertexShader;		// Vertex shader object
unsigned int fragmentShader;	// Fragment shader object
unsigned int shaderProgram;		// Shader Program
unsigned int EBO;				// Element buffer object

// Window settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex Shader in GLSL with input of 3D Vector and output 4D Vector
const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main()
{
	// Initialize GLAD and GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	// Tell the user the minimum requirements of OpenGL Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);	// Tell the user the maximum requirements of OpenGL Version
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// Tell GLFW to use core-profile

	// Initialize a window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "First Window", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);	// Register resize callback

	// Initialize GLAD by load address of OpenGL 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}
	
	// Compile the shader program
	createShaderProgram();

	// Check whether the user clicked the close button or not
	while (!glfwWindowShouldClose(window))
	{
		/*	Input Callback	*/
		processInput(window);

		/* Rendering commands */

		// Clear the screen's color buffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);		// A single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);	// no need to unbind it every time 

		/* Check & call events and swap buffers */
		glfwSwapBuffers(window);	// Swap color buffer
		glfwPollEvents();			// Check if any events are triggered (keyboard, mouse, etc.), then updates the window state, and calls the corresponding functions
	}

	// Delete previous vertex & fragment shader object
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	// Clean GLFW's resources
	glfwTerminate();

	return 0;
}

// Callback when the window size has changed
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Callback to response the window input
void processInput(GLFWwindow* window)
{
	// When 'Escape' Button is pressed then closed the window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// Create shader program by attaching & compiling vertex and fragment shader
void createShaderProgram(void)
{
	// Log for Shader while Compiling
	int success;
	char infoLog[512];

	/*
	// Rectangle vertices
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, // top left 
	};
	*/

	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top mid
		0.5f, -0.5f, 0.0f,	// bottom mid
		-0.5f, -0.5f, 0.0f, // bottom left
		1.0f,  -0.5f, 0.0f, // bottom right
	};


	// Indices for rectangle
	unsigned int indices[] = {
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	/*						Vertex Shader						*/
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);	// Attach source code to the shader object
	glCompileShader(vertexShader);	// Compile the shader

	// Get vertex shader compiling's log
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/*						Fragment Shader						*/
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);	// Attach source code to the shader object
	glCompileShader(fragmentShader); // Compile the shader

	// Get fragment shader compiling's log
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}

	/*					Shader Program for Linking				*/
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Get shader program compiling's log
	glGetShaderiv(shaderProgram, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << endl;
	}

	// Delete Vertex and fragment shader resources
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// 0. Bind Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	// 1. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);												// Bind newly created VBO to the GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// Copies vertex data into the buffer's memory
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 2. Set the vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// The first parameter related to the vertex shader layout location
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}