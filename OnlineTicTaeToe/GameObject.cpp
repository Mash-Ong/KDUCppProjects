#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{
	m_pos = Vector(0.0f, 0.0f, 0.0f);
	m_rotation = 0.0f;
	m_scale = Vector(1.0f, 1.0f, 1.0f);
	m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	m_blendMode = Sprite::BLEND_ALPHA;
	m_sprite = NULL;
}

GameObject::~GameObject()
{
	//std::cout << "A game object is destructed." << std::endl;
};

GameObject::GameObject(Sprite* sprite) :GameObject::GameObject()
{
	m_sprite = sprite;
};

void GameObject::setPosition(Vector pos)
{
	m_pos = pos;
}

const Vector & GameObject::getPosition()
{
	return m_pos;
}

void GameObject::setScale(Vector scale)
{
	m_scale = scale;
}

Vector GameObject::getScale()
{
	return m_scale;
}

void GameObject::setRotation(float rotation)
{
	m_rotation = rotation;
}

float GameObject::getRotation()
{
	return m_rotation;
}

void GameObject::setSprite(Sprite* sprite)
{
	m_sprite = sprite;
}

Sprite* GameObject::getSprite()
{
	return m_sprite;
}

void GameObject::setColor(const Sprite::Color& color)
{
	m_color = color;
}

const Sprite::Color& GameObject::getColor()
{
	return m_color;
}

void GameObject::setOpacity(float opacity)
{
	m_color.a = opacity;
}

const float GameObject::getOpacity()
{
	return m_color.a;
}

void GameObject::setBlend(Sprite::BlendMode mode)
{
	m_blendMode = mode;
};

Sprite::BlendMode GameObject::getBlendingMode()
{
	return m_blendMode;
}

void GameObject::draw()
{
	if (getSprite())
		getSprite()->draw(m_pos.mVal[0], m_pos.mVal[1], m_scale.mVal[0], m_scale.mVal[1], m_rotation, m_color, m_blendMode);
};

void GameObject::update(float deltaTime) {};