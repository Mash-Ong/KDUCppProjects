#ifndef SPRITE_H
#define SPRITE_H

#include <string>

//! A container for texture and drawing of texture.
class Sprite
{
private:
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_textureID;

public:
	struct Color
	{
		float r;
		float g;
		float b;
		float a;
	};

	enum BlendMode
	{
		BLEND_ALPHA = 0,
		BLEND_ADDITIVE,
		BLEND_MULTIPLY
	};

	Sprite();
	Sprite(const std::string& file);

	~Sprite();
	unsigned int getTextureWidth();  //! Get the width of the texture.
	unsigned int getTextureHeight(); //! Get the height of the texture.

	void draw(float x, float y, float scale_x, float scale_y, float rotation, Color color, BlendMode blend); //! Draw the texture.


};
#endif