#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"
#include "std_image.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const GLuint SCR_WIDTH = 1024;
const GLuint SCR_HEIGHT = 768;

void loadTexture(const char *texturePath, unsigned int &texture, GLint wrapParam = GL_REPEAT, GLint filterParam = GL_LINEAR);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress ) )
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	// ------------------------------------
	Shader firstShader("shader.vs", "shader.fs"); // you can name your shader files however you like

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
   // color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load and create a texture
	unsigned int texture[2]; // texture[2] is a array contains two texture object
	glGenTextures(2, texture);
	loadTexture(".\\images\\container.jpg", texture[0]);   //loadTexture(".\\images\\container.jpg", texture[0], GL_CLAMP_TO_EDGE);
	loadTexture(".\\images\\awesomeface.png", texture[1]);

	// set which sampler2D belongs to a specific texture unit
	firstShader.useShader();
	firstShader.setUniformValue("texture0", 0);  // here set value 0 indicates it maps uniform sampler2D texture0 to TEXTURE0
	firstShader.setUniformValue("texture1", 1); //  here set value 1 indicates it maps uniform sampler2D texture1 to TEXTURE1


	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	// glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		// input process
		// -----
		processInput(window);

		// clear the color buffer which can clear the screen to a color we set in glClearColor function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind texture object
		glActiveTexture(GL_TEXTURE0);  // activate GL_TEXTURE0 to bind it with texture0
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glActiveTexture(GL_TEXTURE1);  // activate GL_TEXTURE1 to bind it with texture1
		glBindTexture(GL_TEXTURE_2D, texture[1]);

		// transform matrix applyed to vertex in vertex shader
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
		trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));

		firstShader.setUniformValue("transform", trans);

		float timeValue = static_cast<float>(glfwGetTime());
		float greenValue = sin(timeValue) / 2.0 + 0.5;
		//glUniform4f(glGetUniformLocation(firstShader.ID, "ourColor"), 0.0f, greenValue, 0.0f, 1.0f);
		//glUniform1f(glGetUniformLocation(firstShader.ID, "val"), greenValue);
		//firstShader.setUniformValue("ourColor",  0.0f, greenValue, 0.0f, 1.0f);
		//firstShader.setUniformValue("val", greenValue);


		// render the triangle
		glBindVertexArray(VAO); // bind the VAO to use it

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// render another container
		trans = glm::mat4(1.0f);  // reset it to an identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5, 0.5, 0.0));
		float scale = sin(glfwGetTime());
		trans = glm::scale(trans, glm::vec3(scale, scale, scale));
		//GLuint uni_location = glGetUniformLocation(firstShader.ID, "transform");
		//glUniformMatrix4fv(uni_location, 1, GL_FALSE, &trans[0][0]);  // this time take the matrix value array's first element as its memory pointer value
		firstShader.setUniformValue("transform", &trans[0][0]);
		// now with the uniform matrix being replaced with new transformations, draw it again.
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

void loadTexture(const char *texturePath, unsigned int &texture, GLint wrapParam, GLint filterParam)
{
	glGenTextures(1, &texture);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(texturePath, &width, &height, &nrComponents, 0);
	if (!data)
	{
		std::cout << "Failed to load texture" << std::endl;
		return;
	}
	else
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		if (nrComponents == 3)
			format = GL_RGB;
		if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapParam);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterParam);
	}

	stbi_image_free(data);
}