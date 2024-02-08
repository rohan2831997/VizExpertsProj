#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"CurveMaker.h"

#include <fstream>

// Vertices coordinates
std::vector<Vertex> vertices;

void readInputfromFile()
{
	std::ifstream infile("input.txt");
	std::string line;
	while (std::getline(infile, line)) {

		//range of x  from 0 to 800
		//range of y from 0 to 400
		float x = 0;
		float y = 0;

		std::string num1 = line.substr(0, line.find(','));
		std::string num2 = line.substr(line.find(',') + 1, line.size());

		x = stoi(num1, 0, 10);
		y = stoi(num2, 0, 10);

		x -= 400.0f;
		x /= 400.0f;

		y -= 200.0f;
		y /= 400.0f;

		if (x == -1.0f)
		{
			x += 0.01f;
		}

		if (x == 1.0f)
		{
			x -= 0.001f;
		}

		if (y == -1.0f)
		{
			y += 0.01f;
		}

		if (y == 1.0f)
		{
			y -= 0.001f;
		}


		Vertex v;
		v.position = glm::vec3{ x,y,0.0f };
		v.color = glm::vec3{ 0.0f,1.0f,0.0f };
		vertices.push_back(v);
	}
}

int width = 1500;
int height = 1000;

int main()
{
	// Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object of 1600 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 1600, y = 800
	glViewport(0, 0, width, height);



	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	//Wireframe Mode
	glPolygonMode(GL_FRONT, GL_LINE);
	glPolygonMode(GL_BACK, GL_LINE);

	//
	readInputfromFile();

	//curvemaker
	curveMaker c(vertices,0.05f);

	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		//// Draw primitives, number of indices, datatype of indices, index of indices
		//glDrawElements(GL_LINES,4, GL_UNSIGNED_INT, 0);
		c.Draw(shaderProgram);
		c.Draw_ext(shaderProgram);
		// Swap the back buffer with the front buffer

		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}