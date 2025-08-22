/*ideas:

	Memory and performance optimization are not a concern at the moment

	use line of best fit algorithm from
	https://github.com/antkowiak/LineOfBestFit/blob/master/lobf.cpp
	to find the line of best fit from the immediate points in like a 2 radius
	and extract a normal vector from that

	to distinguish points that are at the edge, only add the points (black)
	that are next to white points to the list of points which
	will be used to find the line of best fit. 

	Instead of checking if a horse is within collision distance of every boundary,
	only check if within collision distance to points that are within a certain area.
	This would require a map to be stored as as a 2d array, where each point 
	is either a boundary or open space.
*/
#define GLEW_STATIC
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <string>

#include "MapCreator.h"

#include "OpenGL/Renderer.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/VertexBufferLayout.h"
#include "OpenGL/Texture.h"

int main() {
	//set up window and buttons
	//load and process image
	//TODO: subject to change once a map selection menu is added
	std::string filepath = "res/maps/testimage.png";
	HorseMap map = HorseMap(filepath);
	//run simulation

	GLFWwindow* window;

	//Initialize the library
	if (!glfwInit()) {
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) std::cout << "yu facked up" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		//pixel positions
		/*float positions[] = {	100.0f, 100.0f,	0.0f, 0.0f,	//0
								860.0f, 100.0f, 1.0f, 0.0f,	//1
								100.0f, 440.0f, 0.0f, 1.0f, //2
								860.0f, 440.0f, 1.0f, 1.0f	//3							
		};*/
		
		//relative positions
		float positions[] = {   -1.0f, -1.0f, 0.0f, 0.0f, //0
								1.0f,  -1.0f, 1.0f, 0.0f, //1
								-1.0f, 1.0f,  0.0f, 1.0f, //2
								1.0f,  1.0f,  1.0f, 1.0f  //3
		};

		unsigned int indices[] = {
			0, 1, 2,	//first triangle
			1, 3, 2		//second triangle
		};

		//Create Vertex Array Object
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		constexpr int POINTS = 4;
		constexpr int FLOATS_PER_POINT = 4;

		VertexArray va;
		VertexBuffer vb(positions, POINTS * FLOATS_PER_POINT * sizeof(float));

		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2); //4 floats per point (x, y, u, v)
		va.AddBuffer(vb, layout);
		va.Bind();

		// Create Index Buffer Object
		IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

		// projections not neccesary at the moment
		/*
		// Create Projection
		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		// these two can cancel each other out
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::vec3 translation(0, 0, 0);
		*/

		Shader shader("res/shaders/basic.shader");
		shader.Bind();

		Texture texture("res/maps/testimage.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		va.Unbind();
		shader.Unbind();
		vb.Unbind();
		ib.Unbind();

		
		int width0, height0;
		glfwGetWindowSize(window, &width0, &height0);

		Renderer renderer;

		//Loop until the user closes the window
		while (!glfwWindowShouldClose(window)) {

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			int width1, height1;
			glfwGetWindowSize(window, &width1, &height1);

			//Update translation if resized
			/*
			if (width0 != width1 || height0 != height1) {

				width0 = width1;
				height0 = height1;

				//Update projection matrix
				proj = glm::ortho(0.0f, (float)width0, 0.0f, (float)height0, -1.0f, 1.0f);
			}

			glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
			glm::mat4 mvp = proj * view * model;
			*/

			//Update viewport if resized
			if (width0 != width1 || height0 != height1) {
				width0 = width1;
				height0 = height1;
				glViewport(0, 0, width0, height0);
			}

			shader.Bind();
			//shader.SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(va, ib, shader);
			
			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	//Clean up
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}