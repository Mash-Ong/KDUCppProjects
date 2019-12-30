#ifndef  _BUTTON_H
#define _BUTTON_H

#include "GameObject.h"

class Sprite;

class Button :public GameObject
{
public:
	float m_width;
	float m_height;
	bool m_hovered; /* is the mouse cursor over the control? */

	Button();
	Button(float posX, float posY, float width, float height, Sprite* sprite);
	bool isHovered(float mouseX, float mouseY);
	void draw();
};
#endif // ! _BUTTON_H
