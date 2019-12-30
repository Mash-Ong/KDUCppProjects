#ifndef _CELL_H
#define _CELL_H

#include "GameObject.h"

class Button;

//! The cell of the grid. A cell contains two game objects.
class Cell :public GameObject
{
public:
	enum Status
	{
		UNCHECKED = 0,
		CIRCLED = 1,
		CROSSED = 2
	};

	Cell();
	Cell(float posX, float posY, float width, float height, Status state, Sprite* bgSprite, Sprite* fgSprite);
	~Cell();
	
	void update(float deltaTime);
	void draw();

	Status getStatus();
	void setStatus(Status newStatus, Sprite* sprite); // Change the cell's status among unchecked, ticked and crossed.
	void setHovered(bool newHovered);
	bool isHovered(float mouseX, float mouseY);
	bool isClickable();

private:
	float m_width;
	float m_height;
	Status m_status;
	Button* m_bg; // the derived game object to represent the background image of this cell and also its clickable area.
};
#endif // !_CELL_H
