//#include "Resources/GLEW/glew.h"
#include "Resources/GLFW/glfw3.h"

#include "display.h"
#include <iostream>
#include <stdio.h>
#include <thread>

GLubyte* frameBuffer;

void drawScreen() {
	glRasterPos2d(-1.0, -1.0);
	glDrawPixels(128 * PIXEL_SCALE, 128 * PIXEL_SCALE, GL_RGB, GL_UNSIGNED_BYTE, frameBuffer);
	if (GLenum error = glGetError()) {
		std::cout << "OpenGL Error: " << " ID: " << error << std::endl;
	}
}

void handleClick(GLFWwindow* window, int button, int action, int modifiers) {
	if (button != GLFW_MOUSE_BUTTON_LEFT) {
		return;
	}
	if (action != GLFW_PRESS) {
		return;
	}

	double xPos, yPos;
	glfwGetCursorPos(window, &xPos, &yPos);

	//converting to coordinates
	int x = xPos / PIXEL_SCALE;
	int y = yPos / PIXEL_SCALE;

	std::cout << "Click Coordinates: x=" << x << " y=" << y << std::endl;
}

int main() {
	//Prompting user for com port
	std::cout << "Enter the com port you desire to use (Will use 128kbps). Case sensitive, example: COM3\n";
	std::cout << "Alternatively, type \"D2XX\" to use the direct FTDI driver (Will use 1Mbps).\n";
	char portInput[40];
	std::cin >> portInput;
	if (!initDisplay(portInput)) {
		//error already reported
		return -1;
	}

	//Initializing openGL
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	//I wanted to have a border, but no because opengl uses goddamn floats so my resolution needs to be a multiple of 2
	frameBuffer = (GLubyte*)malloc(128 * 128 * PIXEL_SCALE * PIXEL_SCALE * 3); 

	if (frameBuffer == nullptr) {
		std::cout << "Out of memory, allocate more memory to this program, or close other programs on your PC.\n";
		return -2;
	}

	ZeroMemory(frameBuffer, 128 * 128 * PIXEL_SCALE * PIXEL_SCALE * 3);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(128 * PIXEL_SCALE, 128 * PIXEL_SCALE, "uLCD Emulator", NULL, NULL);
	
	glfwSetMouseButtonCallback(window, handleClick);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	std::thread dispReader (runDisplayLoop);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	{

		glDisable(GL_DEPTH_TEST);

		if (GLenum error = glGetError()) {
			std::cout << "OpenGL Error: " << " ID: " << error << std::endl;
			return 0;
		}

		while (!glfwWindowShouldClose(window))
		{

			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			drawScreen();

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	running = false;
	dispReader.join();

	if (com) {
		CloseHandle(com);
	}
	else if (ftCom) {
		FT_Close(ftCom);
	}
	free(frameBuffer);
	glfwTerminate();
}