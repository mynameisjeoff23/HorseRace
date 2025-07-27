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


#include "MapCreator.h"
#include <string>

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
		//Loop until the user closes the window
		while (!glfwWindowShouldClose(window)) {

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	return 0;
}