#include "Button.h"
#include "Sprite.h"
#include "Vector.h"
#include <iostream>

Button::Button() :GameObject::GameObject()
{
	m_width = 0;
	m_height = 0;
	m_hovered = false;
}

Button::Button(float posX, float posY, float width, float height, Sprite * sprite)
{
	m_width = width;
	m_height = height;
	m_hovered = false;
	setSprite(sprite);
	setPosition(Vector(posX, posY, 0));
	// Scale to affect the size of sprite to match the defined width and height
	setScale(Vector(m_width / getSprite()->getTextureWidth(), m_height / getSprite()->getTextureHeight(), 0));
	setColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	setBlend(Sprite::BLEND_ALPHA);
}

// Return true if the mouse cursor is within this button's area.
bool Button::isHovered(float mouseX, float mouseY)
{
	float x1 = getPosition().mVal[0] - m_width / 2,
		y1 = getPosition().mVal[1] - m_height / 2,
		x2 = getPosition().mVal[0] + m_width / 2,
		y2 = getPosition().mVal[1] + m_height / 2;
	if (mouseX > x1 &&
		mouseX < x2 &&
		mouseY > y1 &&
		mouseY < y2)
		return true;
	return false;
}

void Button::draw()
{
	if (getSprite() != NULL)
	{
		Sprite::Color spriteTint = getColor();

		if (m_hovered)
		{
			spriteTint.r -= 0.25f;
			spriteTint.g -= 0.25f;
			spriteTint.b -= 0.25f;
		}

		getSprite()->draw(getPosition().mVal[0], getPosition().mVal[1], getScale().mVal[0], getScale().mVal[1], getRotation(), spriteTint, getBlendingMode());
	}
}