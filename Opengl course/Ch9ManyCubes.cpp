// make sure that glad comes before glfw

#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "shader_s.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main()
{
	// -------------------------------------------- Start Initialization ------------------------------- //
	glfwInit();
	// set OpenGL version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core mode over immediate mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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
	// -------------------------------------------- End Initialization ------------------------------- //

	// load shaders
	Shader ourShaders("./Shaders/Ch9Cube/vs.glsl", "./Shaders/Ch9Cube/fs.glsl");


	// -------------------------------------------- Start Convert textures ------------------------------- //

	// create a texture object in openGL
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// set wrapping options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// set filtering options (here we use bilinear filtering)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// texture 1 (wood) //

	int width, height, numChannels;

	// tell stb_image.h to flip loaded texture's on the y-axis
	// cuz in images, y=0 is the top. of course it is
	stbi_set_flip_vertically_on_load(true);


	unsigned char* data = stbi_load("images/wood.png", &width, &height, &numChannels, 0);

	// check that it actually worked
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

		// at this point, image (png for example) is loaded in openGL
		// now we can generate mipmaps!
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[textures] stbi image failed!";
		return 0;
	}


	// texture 2 (troll face) //
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	data = stbi_load("images/tf.png", &width, &height, &numChannels, 0);

	// check that it actually worked
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// at this point, image (png for example) is loaded in openGL
		// now we can generate mipmaps!
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "[textures] stbi image failed!";
		return 0;
	}
	// and now free image memory as good practice. We should be done!
	stbi_image_free(data);

	// -------------------------------------------- End Convert textures ------------------------------- //




	// -------------------------------------------- DATA ------------------------------- //
	// vertex data (coordinates range from -1 to 1)
	// ------------------------------------------------------------------ //
	float vertices[] = {
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

	// Rendering 10 cubes, this is where each cube should be at in global space
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	// index data (which point is what vertex of the rectangle)
	// ------------------------------------------------------------------ //


	// model view projection matrices 
	// glm::mat4 model = glm::mat4(1.0f);

	// lets make local space be world space but -55 degrees rotated along x and y-axis
	// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	// model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// for view matrix, we want camera to be 3 z-units above the scene (positive z goes out of the screen, remember)
	// so we will shift the whole scene 3 units DOWN accomplishes same thing
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	// for projection, use a perspective projection with 45 degree FOV and following settings below:
	glm::mat4 projection = glm::mat4(1.0f);
	float fieldOfView = glm::radians(45.0f);
	float nearPlanes = 0.1f;
	float farPlanes = 100.0f;
	projection = glm::perspective(fieldOfView, 800.0f / 600.0f, nearPlanes, farPlanes);

	// --------------------------------------------  ------------------------------- //

	// GL CREATES
	// --------------------------------- //
	unsigned int VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	// --------------------------------- //

	// GL BINDS
	// --------------------------------- //
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// --------------------------------- //

	// GL Populate methods (these functions actually move the data into the OpenGL objects)
	// --------------------------------- //
	// copy the vertex data into this buffer's memory
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// --------------------------------- //





	// TEXTURE ATTRIBUTES aka "telling how the input data to vertex shader is packed"
	// ----------------------------------------------
	// x,y,z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	// s,t (texture coordinates)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
		(void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ----------------------------------------------



	// ------------------------ Set UNIFORMS (aka stuff that GLSL expects to get from CPU) ------------------------------- //
	// set texture units aka assign channels to each texture (so GLSL knows which channel is what texture)

	ourShaders.use();
	glUniform1i(glGetUniformLocation(ourShaders.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(ourShaders.ID, "texture2"), 1);


	glUniformMatrix4fv(glGetUniformLocation(ourShaders.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(ourShaders.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// ------------------------------------------------ END SET UNIFORMS ------------------------------- //

	glEnable(GL_DEPTH_TEST);
	// simple render loop (its just a while loop!)
	while (!glfwWindowShouldClose(window))
	{
		// nicer background color than black
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// define transformation matrix data
		// glm::mat4 mat = glm::mat4(1.0f); // identity
		// mat = glm::translate(mat, glm::vec3(0.5f, 0.5f, 0.0f)); // move center to top right corner
		// mat = glm::rotate(mat, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// pass in the uniform (for transformation matrices, this has to happen every frame)
		// glUniformMatrix4fv(glGetUniformLocation(ourShaders.ID, "transformation_matrix"), 1, GL_FALSE, glm::value_ptr(mat));

		ourShaders.use();
		// draws two triangles
		glBindVertexArray(VAO); 

		// uncomment above for just one cube, this code here is for rendering 10 cubes~!
		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			model = glm::rotate(model, glm::radians(-15.0f * (i + 1) * (float)glfwGetTime()), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(-25.0f * (i + 1) * (float)glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(ourShaders.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		


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