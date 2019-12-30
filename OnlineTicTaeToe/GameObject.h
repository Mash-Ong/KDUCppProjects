#ifndef _GAMEOBJECT_H
#define _GAMEOBJECT_H

#include "Vector.h"
#include "Sprite.h"

//! The base class for objects that can be transformed on the screen.
class GameObject
{
private:
	Sprite* m_sprite;
	Vector m_pos;
	float m_rotation;
	Vector m_scale;
	Sprite::Color m_color;
	Sprite::BlendMode m_blendMode;

public:
	GameObject();
	~GameObject();
	GameObject(Sprite* sprite);

	void setPosition(Vector pos); //! Set the position of this game object
	const Vector& getPosition(); //! Get the position of this game object
	void setRotation(float rotation); //! Set angle of rotation
	float getRotation(); //! Get angle of rotation
	void setScale(Vector scale); //! Set scale
	Vector getScale(); //! Get scale
	void setSprite(Sprite* sprite); //! Set sprite
	Sprite* getSprite(); //! Get sprite
	void setColor(const Sprite::Color& color); //! Set the color tinting of the texture
	const Sprite::Color& getColor(); //! Get color
	void setOpacity(float opacity);
	const float getOpacity();

	void setBlend(Sprite::BlendMode mode); //! Set how the texture are blended. OPTIONS: BLEND_ALPHA, BLEND_ADDITIVE, BLEND_MULTIPLY
	Sprite::BlendMode getBlendingMode(); //! Get blending mode [0 - BLEND_ALPHA, 1 - BLEND_ADDITIVE, 2 - BLEND_MULTIPLY]

	virtual void draw(); //! Draw sprite
	virtual void update(float deltaTime); //! To be overriden
};
#endif // !_GAMEOBJECT_H