#include <iostream>
#include "MyPhoton.h"

#include <windows.h>
#include <GL/GLU.h>
#include <GLFW/glfw3.h>
#include "Application.h"

#define M_2PI 6.28318530718

using namespace std;

const int RESOLUTION_X = 800; // the desired resolution when the app is loaded.
const int RESOLUTION_Y = 600;
int m_winWidth;
int m_winHeight;

void onWindowResized(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	m_winWidth = width;
	m_winHeight = height;

	//! Use ortho2D view
	gluOrtho2D(0, width, 0, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application::getInstance().onMouseMoved(xpos, m_winHeight - ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Application::getInstance().onMouseButton(button, action);
}

void DrawCircle(float x, float y, float radius, int resolution)
{
	float theta = M_2PI / resolution;

	for (int i = 0; i < resolution; i++)
	{
		float x1 = x + sin(i*theta) * radius;
		float y1 = y + cos(i*theta) * radius;

		float x2 = x + sin((i+1)*theta) * radius;
		float y2 = y + cos((i+1)*theta) * radius;

		glBegin(GL_TRIANGLES);
		glVertex3f(x, y, 0);
		glVertex3f(x1, y1, 0);
		glVertex3f(x2, y2, 0);
		glEnd();
	}
}

int main (void)
{
	GLFWwindow* window;
	double presentTime = glfwGetTime(), pastTime = presentTime;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create m_a windowed mode window and its OpenGL context */
	window = glfwCreateWindow (RESOLUTION_X, RESOLUTION_Y, "", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwSetWindowSizeCallback (window, onWindowResized);
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	onWindowResized (window, RESOLUTION_X, RESOLUTION_Y);

	MyPhoton::getInstance().connect();

	glfwSetKeyCallback(window, key_callback);

	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	
	Application::getInstance().init();

	while (!glfwWindowShouldClose(window))
	{
		MyPhoton::getInstance().run();

		presentTime = glfwGetTime();
		Application::getInstance().update((presentTime - pastTime));
		Application::getInstance().draw();
		pastTime = presentTime;

		glfwSwapBuffers (window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}