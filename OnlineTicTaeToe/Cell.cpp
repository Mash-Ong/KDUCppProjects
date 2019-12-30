#include "Cell.h"
#include "Button.h"
#include "Sprite.h"
#include <iostream>

Cell::Cell()
{
	float m_width = 0;
	float m_height = 0;
	m_bg = new Button();
	setStatus(UNCHECKED, NULL);
}

Cell::Cell(float posX, float posY, float width, float height, Status status, Sprite* bgSprite, Sprite * fgSprite)
{
	setPosition(Vector(posX, posY, 0));
	m_bg = new Button(posX, posY, width, height, bgSprite); // this object is being scaled to fit the defined width and height in its constructor.
	setScale(m_bg->getScale() - Vector(0.1f, 0.1f, 0.0f)); // to make the cross and circle sprites to be drawn on top of the background looks smaller.
	setStatus(status, fgSprite);
}

Cell::~Cell()
{

}

void Cell::update(float deltaTime)
{

}

void Cell::draw()
{
	m_bg->draw();
	GameObject::draw();
}

Cell::Status Cell::getStatus()
{
	return m_status;
}

void Cell::setStatus(Status newStatus, Sprite * sprite)
{
	m_status = newStatus;
	setSprite(sprite);
}

void Cell::setHovered(bool newHovered)
{
	m_bg->m_hovered = newHovered;
}

bool Cell::isHovered(float mouseX, float mouseY)
{
	return m_bg->isHovered(mouseX, mouseY);
}

bool Cell::isClickable()
{
	return m_status == UNCHECKED;
}
