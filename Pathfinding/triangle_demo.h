#ifndef _TRIANGLE_DEMO_H
#define _TRIANGLE_DEMO_H

#include "demo_base.h"
#include <iostream>

class TriangleDemo : public DemoBase
{
private:

public:
	void init()
	{
	
	}

	void deinit()
	{
	}

	void draw()
	{
		//test drawing
		glBegin(GL_TRIANGLES);
			glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(0, 0, 0);
			glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0, 500, 0);
			glColor3f(0.0f, 0.0f, 1.0f); glVertex3f(500, 500, 0);
		glEnd();
	}

	void onMouseButton(int button, int action)
	{
		std::cout << "mouse button : " << button << ", " << action << std::endl;
	}

	void onMouseMove(double x, double y)
	{
		std::cout << "mouse pos : " << x << ", " << y << std::endl;
	}

};

#endif
