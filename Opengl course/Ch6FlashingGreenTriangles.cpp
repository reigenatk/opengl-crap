// make sure that glad comes before glfw

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	glfwInit();

	// set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core mode over immediate mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// only mac users need this
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// intitialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set viewport (lower left, lower right, width, height)
	glViewport(0, 0, 800, 600);

	// register a callback that will reset the viewport each time window size changes
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// vertex data (coordinates range from -1 to 1)
	// ------------------------------------------------------------------
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f, // left  
		 0.0f, -0.5f, 0.0f, // right 
		 -0.45f,  0.5f, 0.0f,  // top
		// second triangle
		 0.0f, -0.5f, 0.0f, // left  
		 0.9f, -0.5f, 0.0f, // right 
		 0.45f,  0.5f, 0.0f  // top   
	};

	// create vertex buffer object
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	// Personal note: First bind the VAO!
	// Binds the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	// bind created buffer to GL_ARRAY_BUFFER target
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// copy the vertex data into this buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// GLSL source code for vertex shader
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// create the vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// set its GLSL source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// and compile it! Now we should have a vertex shader
	glCompileShader(vertexShader);

	// check to see if the GLSL compilation worked (you can do this!)
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// if it fails, say so
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// GLSL source for a fragment shader (this just outputs a orange color for each pixel)
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = ourColor;\n"
		"}\n\0";

	// create the frag shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	// set its GLSL source code
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

	// compile
	glCompileShader(fragmentShader);

	// check to see if the GLSL compilation worked 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// if it fails, say so
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// make a shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// tell the program where our shaders are
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// link and run!
	glLinkProgram(shaderProgram);

	// check for errors during linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}



	// delete shaders once linked cuz we dont need them afterwards
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// specify that the data from vertex buffer object comes in a tightly packed sequence of 3 values, so that vertex buffer knows how to handle it
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	// unbind the VBO and VAOs below
	// ------------------------------------
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	// ------------------------------------

	// simple render loop (its just a while loop!)
	while (!glfwWindowShouldClose(window))
	{

		glUseProgram(shaderProgram);

		// set the frag shader color
		float time = glfwGetTime();
		float green = sin(time) / 2.0f + 0.5f;

		// get a reference to the uniform
		int fragColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		// set its value
		glUniform4f(fragColorLocation, 0.0f, green, 0.0f, 1.0f);


		// draws two triangles
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 6); // draw from vertex 0, and draw 3 vertices

		// does a double buffer swap to avoid flickering
		glfwSwapBuffers(window);

		// process any keypresses
		glfwPollEvents();
	}

	// de allocate stuff (here its the VBO and VAOs)
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// close the application 
	glfwTerminate();
	return 0;
}

// create a function that runs eachtime window size changes
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}