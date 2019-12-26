#ifndef  _BUTTON_H
#define _BUTTON_H

typedef void(*ButtonCallback)(); // A function pointer type for buttons.

class Button
{
public:
	float x;
	float y;
	float w;
	float h;
	GLuint textureID; // loaded texture to display
	bool hovered; /* is the mouse cursor over the control? */
	ButtonCallback callbackFunc; // A pointer to a function to call if the button is pressed

	Button() {};

	Button(float posX, float posY, float width, float height, GLuint _textureID, ButtonCallback callback)
	{
		x = posX;
		y = posY;
		w = width;
		h = height;
		textureID = _textureID;
		hovered = false;
		callbackFunc = callback;
	}

	// Return true if the mouse cursor is within this button's area.
	virtual bool isHovered(float mouseX, float mouseY)
	{

		if (mouseX > x &&
			mouseX < x + w &&
			mouseY > y &&
			mouseY < y + h)
			return true;
		return false;
	}

	virtual void draw() 
	{
		if (hovered)
			glColor3f(1.0f, 1.0f, 1.0f);
		else
			glColor3f(0.5f, 0.5f, 0.5f);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + h, 0);

			glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + h, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y + h, 0);
		glEnd();
	}
	virtual void onMouseMove(float mouseX, float mouseY) { hovered = isHovered(mouseX, mouseY); }
	virtual void onMouseButton(int btnIndex, int btnAction)
	{ 
		if (hovered)
		{
			if (btnIndex == 0 && btnAction == 0)
				callbackFunc();
		}
	}
	virtual float getValue() { return 0.0f; } // exclusively used by slider
};
#endif // ! _BUTTON_H

class CheckBox : public Button
{
private:
	int checkedTexID;
	int uncheckedTexID;
	bool checked;

	void setChecked(bool _checked)
	{
		checked = _checked;
		textureID = checked ? checkedTexID : uncheckedTexID;
	}

public:
	// cTextureID refers to the sprite when the check box is checked; uCTextureID refers to its unchecked sprite.
	CheckBox(float posX, float posY, float width, float height, GLuint cTextureID, GLuint uCTextureID, bool _checked, ButtonCallback callback)
	{
		x = posX;
		y = posY;
		w = width;
		h = height;
		checkedTexID = cTextureID;
		uncheckedTexID = uCTextureID;
		hovered = false;
		setChecked(_checked);
		callbackFunc = callback;
	}
	~CheckBox() {};
	void onMouseButton(int btnIndex, int btnAction)
	{
		if (hovered)
		{
			if (btnIndex == 0 && btnAction == 0)
			{
				callbackFunc();
				setChecked(!checked);
			}
		}
	}
};

// change its texture on clicked
class ToggleButton : public Button
{
private:
	int texIDA;
	int texIDB;

	void switchTexture()
	{
		textureID = textureID == texIDA ? texIDB : texIDA;
	}

public:
	ToggleButton(float posX, float posY, float width, float height, GLuint TextureIDA, GLuint TextureIDB, ButtonCallback callback)
	{
		x = posX;
		y = posY;
		w = width;
		h = height;
		texIDA = TextureIDA;
		textureID = texIDA;
		texIDB = TextureIDB;
		hovered = false;
		callbackFunc = callback;
	}

	void onMouseButton(int btnIndex, int btnAction)
	{
		if (hovered)
		{
			if (btnIndex == 0 && btnAction == 0)
			{
				callbackFunc();
				switchTexture();
			}
		}
	}
};

class Slider : public Button
{
private:
	bool dragged = false;
	float minVal;
	float maxVal;
	float value;
	float knobX, knobMinX, knobMaxX, knobY, knobW, knobH;
	GLuint knobTextureID;

	void defineKnobSize()
	{
		knobY = y + (h / 2);
		knobMinX = x + 1.7 * w / 10;
		knobMaxX = x + 7.5 * w / 10;
		knobW = w / 10;
		knobH = h / 3;
	}

public:
	Slider(float posX, float posY, float width, float height, GLuint bgTexID, GLuint knobTexID, ButtonCallback callback, float _minVal, float _maxVal, float _value)
	{
		x = posX;
		y = posY;
		w = width;
		h = height;
		defineKnobSize();
		textureID = bgTexID;
		knobTextureID = knobTexID;
		callbackFunc = callback;
		minVal = _minVal;
		maxVal = _maxVal;
		setValue(_value);
	}

	virtual bool isHovered(float mouseX, float mouseY)
	{

		if (mouseX > knobX - knobW / 2 &&
			mouseX < knobX + knobW / 2 &&
			mouseY > knobY - knobH / 2 &&
			mouseY < knobY + knobH / 2)
			return true;
		return false;
	}

	void draw()
	{
		if (hovered)
			glColor3f(1.0f, 1.0f, 1.0f);
		
		else
			glColor3f(0.5f, 0.5f, 0.5f);

		// draw the slider background before drawing the knob on top of it.
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,textureID);
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + h, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y + h, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y + h, 0);
		glEnd();

		glBindTexture(GL_TEXTURE_2D, knobTextureID);
		glBegin(GL_TRIANGLES);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(knobX - knobW / 2, knobY - knobH / 2, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(knobX + knobW / 2, knobY - knobH / 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(knobX - knobW / 2, knobY + knobH / 2, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(knobX + knobW / 2, knobY - knobH / 2, 0);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(knobX - knobW / 2, knobY + knobH / 2, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(knobX + knobW / 2, knobY + knobH / 2, 0);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	void onMouseMove(float mouseX, float mouseY)
	{
		hovered = isHovered(mouseX, mouseY);
		if (dragged)
		{
			if (!hovered) // the mouse must be inside the knob area in order to drag the knob
			{
				dragged = false;
				return;
			}
			knobX = mouseX;
			knobX = knobX < knobMinX ? knobMinX : knobX;
			knobX = knobX > knobMaxX ? knobMaxX : knobX;
			float sliderpct = (knobX - knobMinX) / (knobMaxX - knobMinX); // percentage (ranged from 0 to 1)
			value = sliderpct * (maxVal - minVal);
			callbackFunc();
		}
	}

	void onMouseButton(int btnIndex, int btnAction)
	{
		if (hovered)
		{
			if (btnIndex == 0 && btnAction == 1) // pressed left mouse btn
				dragged = true;
			else if (btnIndex == 0 && btnAction == 0) // released left mouse btn
				dragged = false;
		}
	}

	void setValue(float newValue)
	{
		value = newValue;
		value = value < minVal ? minVal : value;
		value = value > maxVal ? maxVal : value;
		float valuepct = (value - minVal) / (maxVal - minVal); // percentage (ranged from 0 to 1)
		knobX = valuepct * (knobMaxX - knobMinX) + knobMinX;
	}

	float getValue()
	{
		return value;
	}
};