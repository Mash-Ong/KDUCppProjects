#ifndef _GRID_H
#define _GRID_H

#include "Vector.h"

class Cell;
class Sprite;

class Grid
{
private:
	float m_posX = 530;
	float m_posY = 260;

	int m_row;
	int m_col;

	float m_cellWidth;
	float m_cellHeight;
	float m_cellSpacing;
	Cell* m_cells;

	

public:
	Grid();
	Grid(int row, int col);
	Grid(int row, int col, float cellWidth, float cellHeight, float spacing);
	~Grid();

	int getRowCount();
	int getColCount();

	float getGridWidth(); // Calculate and return the grid width
	float getGridHeight(); // Calculate and return the grid height

	void initCells();
	Cell* getCell(int cellIndex);
	void draw();
	int onMouseMoved(float mouseX, float mouseY); // return the index of the cell that is being hovered. -1 = no cell
	int onClicked(Vector mousePos); // return the index of the cell that is being clicked. -1 = no cell
};
#endif