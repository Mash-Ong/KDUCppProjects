#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/GLU.h>

#include "camera.h"
#include "matrix.h"
#include "vector.h"

#include "pathfinding.h"
#include <iostream>

#define WINDOW_WIDTH 800 // 1360 // 1600 // 1768 // 1920
#define WINDOW_HEIGHT 680 // 768 // 900 // 992 // 1080
#define TEXTURE_COUNT 9
#define BUTTON_COUNT 6

PathFinding* demo;
Button *m_btns[BUTTON_COUNT];
GLuint GtextureID[TEXTURE_COUNT];

void loadTexture(const char* path, GLuint textureID)
{
	CBitmap bitmap(path);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // apply texture wrapping along horizontal part.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // apply texture wrapping along vertical part.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilinear near filtering. (For when texture needs to scale up on screen) 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // bilinear far filtering. (For when texture needs to scale down on screen)

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());
}

void onWindowResized(GLFWwindow* window, int width, int height)
{
	if (height == 0) height = 1;						// Prevent A Divide By Zero By making Height Equal One

	glViewport(0, 0, width, height);					// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	//gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	gluOrtho2D(0, width, height, 0);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void playBtnCallback()
{
	demo->clearGrid();
	demo->startSearching();
}

void clearPathBtnCallback()
{
	demo->clearGrid();
}

void clearWallBtnCallback()
{
	demo->clearWalls();
}

void diagonalCheckBoxCallback()
{
	demo->toggleDiagonalSearch();
}

void algorithmBtnCallback()
{
	demo->toggleAlgorithm();
}

void weightSliderCallback()
{
	demo->aStarWeight = m_btns[5]->getValue();
}

void initButtons()
{
	// load the buttons' texture
	glGenTextures(TEXTURE_COUNT, GtextureID);
	loadTexture("../media/search_button.bmp", GtextureID[0]);
	loadTexture("../media/clear_path_button.bmp", GtextureID[1]);
	loadTexture("../media/clear_walls_button.bmp", GtextureID[2]);
	loadTexture("../media/cDiagonal_button.bmp", GtextureID[3]);
	loadTexture("../media/uCDiagonal_button.bmp", GtextureID[4]);
	loadTexture("../media/dijkstra_button.bmp", GtextureID[5]);
	loadTexture("../media/aStar_button.bmp", GtextureID[6]);
	loadTexture("../media/slider.bmp", GtextureID[7]);
	loadTexture("../media/slider_knob.bmp", GtextureID[8]);
	
	float btnW = WINDOW_WIDTH * 0.144f, btnH = WINDOW_HEIGHT * 0.065f, posX = WINDOW_WIDTH - btnW - 5.0f;//posY = WINDOW_HEIGHT - btnH - 5.0f;
	
	m_btns[0] = new Button(posX, WINDOW_HEIGHT * 0.360f, btnW, btnH, GtextureID[0], playBtnCallback);
	m_btns[1] = new Button(posX, WINDOW_HEIGHT * 0.440f, btnW, btnH, GtextureID[1], clearPathBtnCallback);
	m_btns[2] = new Button(posX, WINDOW_HEIGHT * 0.520f, btnW, btnH, GtextureID[2], clearWallBtnCallback);
	m_btns[3] = new CheckBox(posX, WINDOW_HEIGHT * 0.088f, btnW, btnH, GtextureID[3], GtextureID[4], false, diagonalCheckBoxCallback);
	m_btns[4] = new ToggleButton(posX, WINDOW_HEIGHT * 0.008f, btnW, btnH, GtextureID[5], GtextureID[6], algorithmBtnCallback);
	m_btns[5] = new Slider(posX, WINDOW_HEIGHT * 0.168f, btnW, btnH, GtextureID[7], GtextureID[8], weightSliderCallback, 0, 10, demo->aStarWeight);
}

void drawButtons()
{
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		m_btns[i]->draw();
	}
}

// Return true if the mouse is within button area.
bool isCursorOverButton(Button *_btn, double mouseX, double mouseY)
{
	if (mouseX > _btn->x &&
		mouseX < _btn->x + _btn->w &&
		mouseY > _btn->y &&
		mouseY < _btn->y + _btn->h)
		return true;
	return false;
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}

void onMouseMove(GLFWwindow* window, double x, double y)
{
	demo->onMouseMove(x, y);

	// Check if the player moves the mouse over any of the buttons
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		m_btns[i]->onMouseMove(x, y);
	}
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	demo->onMouseButton(button, action);

	// Check if the player triggered any of the buttons
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		m_btns[i]->onMouseButton(button, action);
	}
}

int main()
{
	int running = GL_TRUE;

    GLFWwindow* window;

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

	// Open an OpenGL window
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

	// Hook window resize.
	glfwSetWindowSizeCallback(window, onWindowResized);
	   
	/* Make the window's context current */
    glfwMakeContextCurrent(window);

	onWindowResized(window, WINDOW_WIDTH, WINDOW_HEIGHT);

	// hook mouse move callback and lock/hide mouse cursor.
	glfwSetCursorPosCallback(window, onMouseMove);
	glfwSetMouseButtonCallback(window, onMouseButton);
	//glfwSetKeyCallback(window, onKeyCallback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize OpenGL.
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	// initialize demo.
	demo = new PathFinding(WINDOW_WIDTH, WINDOW_HEIGHT);
	demo->init();
	initButtons();

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// OpenGL rendering goes here...
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Check if ESC key was pressed
		if(glfwGetKey(window, GLFW_KEY_ESCAPE))
			break;

		demo->draw();
		drawButtons();

		// Swap front and back rendering buffers
		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	demo->deinit();
	delete demo;

	// Close window and terminate GLFW
	glfwTerminate();
}
