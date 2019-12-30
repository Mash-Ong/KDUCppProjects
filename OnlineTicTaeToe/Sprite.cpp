#include "Sprite.h"
#include "Bitmap.h"
#include "GLFW/glfw3.h"

Sprite::Sprite()
{
	m_width = 60.0f;
	m_height = 60.0f;
	m_textureID = -1;
}

Sprite::Sprite(const std::string& file)
{
	static unsigned int TID = 0;

	// loading the texture
	CBitmap bitmap(file.c_str());

	m_width = bitmap.GetWidth();
	m_height = bitmap.GetHeight();
	m_textureID = TID;
	std::cout << m_textureID << std::endl;
	glBindTexture(GL_TEXTURE_2D, TID); // create or use a named texture

	//*Based on khronos documentation, GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_T is initially set to GL_REPEAT.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//*Based on khronos documentation, GL_TEXTURE_MAG_FILTER is initially set to GL_LINEAR while GL_TEXTURE_MIN_FILTER is initially set to GL_NEAREST_MIPMAP_LINEAR.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // texture magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // texture minification

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap.GetBits());  // define a 2D texture image
	TID++;
}

Sprite::~Sprite()
{
	std::cout << "Sprite deleted!!!!!!!!!!!!!!!!!!!!!" << std::endl;
}

unsigned int Sprite::getTextureWidth() { return m_width; }
unsigned int Sprite::getTextureHeight() { return m_height; }

void Sprite::draw(float x, float y, float scale_x, float scale_y, float rotDeg, Color color, BlendMode blend)
{
	glColor4f(color.r, color.g, color.b, color.a);

	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	switch (blend)
	{
	case BLEND_ALPHA:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		break;
	case BLEND_ADDITIVE:
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		break;
	case BLEND_MULTIPLY:
		glBlendFunc(GL_DST_COLOR, GL_ZERO);
		break;
	}

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_textureID);
	float rotRad = (rotDeg) * (3.141592 / 180.0);
	float s = sin(rotRad);
	float c = cos(rotRad);

	float halfWidth = 0.5f * m_width * scale_x;
	float halfHeight = 0.5f * m_height * scale_y;

	float coord0_x = c * (-halfWidth) - s * (-halfHeight) + x; // bottom left x coordinates of the texture
	float coord0_y = c * (-halfHeight) + s * (-halfWidth) + y; // bottom left y coordinates of the texture
	float coord1_x = c * (halfWidth)-s * (halfHeight)+x; // top right x coordinates of the texture
	float coord1_y = c * (halfHeight)+s * (halfWidth)+y; // top right y coordinates of the texture
	float coord2_x = c * (-halfWidth) - s * (halfHeight)+x; // top left x coordinates of the texture
	float coord2_y = c * (halfHeight)+s * (-halfWidth) + y; // top left y coordinates of the texture
	float coord3_x = c * (halfWidth)-s * (-halfHeight) + x; // bottom right x coordinates of the texture
	float coord3_y = c * (-halfHeight) + s * (halfWidth)+y; // bottom right y coordinates of the texture

	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(coord0_x, coord0_y, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(coord3_x, coord3_y, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(coord2_x, coord2_y, 0);

	glTexCoord2f(1.0f, 0.0f); glVertex3f(coord3_x, coord3_y, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(coord2_x, coord2_y, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(coord1_x, coord1_y, 0);
	glEnd();

	glDisable(GL_BLEND);
}