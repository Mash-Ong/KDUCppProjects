#include "Grid.h"
#include "GameObject.h"
#include "Cell.h"
#include <iostream>

Grid::Grid()
{
	m_cellWidth = 10;
	m_cellHeight = 10;
	m_cellSpacing = 1;
}

Grid::Grid(int row, int col) :Grid::Grid()
{
	m_row = row;
	m_col = col;
	m_cells = new Cell[m_row*m_col];
	initCells();
}

Grid::Grid(int row, int col, float cellWidth, float cellHeight, float spacing)
{
	m_row = row;
	m_col = col;
	m_cellWidth = cellWidth;
	m_cellHeight = cellHeight;
	m_cellSpacing = spacing;
	m_cells = new Cell[m_row*m_col];
	initCells();
}

Grid::~Grid()
{
	delete[] m_cells;
}

int Grid::getRowCount()
{
	return m_row;
}

int Grid::getColCount()
{
	return m_col;
}

float Grid::getGridWidth()
{
	return (m_col * m_cellWidth) + ((m_col - 1) * m_cellSpacing);
}

float Grid::getGridHeight()
{
	return (m_row * m_cellHeight) + ((m_row - 1) * m_cellSpacing);
}

void Grid::initCells()
{
	Sprite* cellBg = new Sprite("../media/gridCell.bmp");

	int lastIndex = m_row * m_col;

	int r, c; // row and column index of a cell
	float pX, pY; // x and y position of the cell
	float sX, sY; // x and y scale of the cell ()

	for (int i = 0; i < lastIndex; i++)
	{
		r = i % m_col;
		c = i / m_row;
		pX = m_cellWidth * c + (c*m_cellSpacing) + (m_cellWidth / 2) + m_posX - (getGridWidth() / 2);
		pY = m_cellHeight * r + (r*m_cellSpacing) + (m_cellHeight / 2) + m_posY - (getGridHeight() / 2);
		m_cells[i] = Cell(pX, pY, m_cellWidth, m_cellHeight, Cell::Status::UNCHECKED, cellBg, NULL);
	}
}

Cell* Grid::getCell(int cellIndex)
{
	return &m_cells[cellIndex];
}

void Grid::draw()
{
	int lastIndex = (m_row * m_col);
	for (int i = 0; i < lastIndex; i++)
	{
		m_cells[i].draw();
	}
}

int Grid::onMouseMoved(float mouseX, float mouseY)
{
	int cellIndex = -1; // -1 means invalid cell.
	int lastIndex = (m_row * m_col);

	for (int i = 0; i < lastIndex; i++)
	{
		if (m_cells[i].isHovered(mouseX, mouseY))
		{
			cellIndex = i;
			break;
		}
		else
		{
			m_cells[i].setHovered(false);
		}
	}
	return cellIndex;
}

int Grid::onClicked(Vector mousePos)
{
	int cellIndex = -1; // -1 means invalid cell.

	// Check whether the mouse is inside the grid area.
	if (mousePos.mVal[0] > m_posX - getGridWidth() / 2 &&
		mousePos.mVal[1] > m_posY - getGridHeight() / 2 &&
		mousePos.mVal[0] < m_posX + getGridWidth() / 2 &&
		mousePos.mVal[1] < m_posY + getGridWidth() / 2)
	{
		int lastIndex = (m_row * m_col);
		for (int i = 0; i < lastIndex; i++)
		{
			if (m_cells[i].isHovered(mousePos.mVal[0], mousePos.mVal[1]) && m_cells[i].isClickable())
			{
				cellIndex = i;
				break;
			}
		}
	}
	
	return cellIndex;
}
