#ifndef _GRID_H
#define _GRID_H

#include "node.h"
#include "vector.h"
#include <list>
#include <iostream>
#include <GLFW/glfw3.h>

using namespace std;

typedef float(*distanceFunc)(int x1, int y1, int x2, int y2);

float euclideanDistance(int x1, int y1, int x2, int y2)
{
	return sqrtf((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}

float manhattanDistance(int x1, int y1, int x2, int y2)
{
	return abs(x2 - x1) + abs(y2 - y1);
}

class Grid
{
private:
	// datas to draw the grid
	int m_posX;
	int m_posY;
	int m_width;
	int m_height;
	int m_row;
	int m_col;
	int m_maxRow;
	int m_maxCol;
	int m_cellGap = 1; // vertical and horizontal spacing between cells.
	int m_cellSize;
	int m_minCellSize = 20;
	int m_vertexCount;
	GLfloat* m_vertices = NULL;
	GLfloat* m_colors = NULL;

	// the elements in the map
	Node* m_nodes = NULL;
	list<Node*> m_openList; // a list of nodes to evaluate whether it is the target.
	Node* m_source = NULL;
	Node* m_target = NULL;
	Node* m_curNode = NULL; // current node in which its neighbors is being checked.
	Vector* m_pathPoints = NULL;
	int m_pathPointCount;
	int m_maxUpf = 5; // maximum number of node being updated or settled per frame

public:
	Grid(int posX, int posY, int width, int height)
	{
		m_posX = posX;
		m_posY = posY;
		m_width = width;
		m_height = height;
	}

	void initGrid()
	{
		m_maxRow = m_width / m_minCellSize;
		m_maxCol = m_height / m_minCellSize;

		m_maxRow = roundf((m_width - m_cellGap * (m_maxRow - 1)) / m_minCellSize);
		m_maxCol = roundf((m_height - m_cellGap * (m_maxCol - 1)) / m_minCellSize);

		m_nodes = new Node[m_maxRow * m_maxCol];

		initNodes();
		scaleGridCell(1);

		// Initialize vertex & color arrays
		m_vertexCount = m_maxRow * m_maxCol * 6;
		m_vertices = new GLfloat[m_vertexCount * 2];
		m_colors = new GLfloat[m_vertexCount * 3];
	}

	void scaleGridCell(int _factor)
	{
		m_cellSize = m_minCellSize * _factor;

		// calculate the number of cells (row * col) to be drawn on the screen.
		m_row = m_width / m_cellSize;
		m_col = m_height / m_cellSize;

		m_row = (m_width - m_cellGap * (m_row - 1)) / m_cellSize;
		m_col = (m_height - m_cellGap * (m_col - 1)) / m_cellSize;

		cout << " Current row count : " << m_row << " Current column count : " << m_col << endl;
	}

	void initNodes() 
	{
		for (int i = 0; i < m_maxRow; i++)
		{
			for (int j = 0; j < m_maxCol; j++)
			{
				m_nodes[(i * m_maxCol) + j].index = (i * m_maxCol) + j;
			}
		}
		setSource(&m_nodes[(m_maxRow / 3) * m_maxCol + (m_maxCol / 2)]);
		setTarget(&m_nodes[(m_maxRow / 3 * 2) * m_maxCol + (m_maxCol / 2)]);
	}

	void deinit()
	{
		// Free memory allocated for the dynamic arrays.
		delete[] m_nodes;
		delete[] m_vertices;
		delete[] m_colors;
		delete[] m_pathPoints;

		// Clear to prevent using invalid memory reference.
		m_nodes = NULL;
		m_vertices = NULL;
		m_colors = NULL;
		m_pathPoints = NULL;
	}

	// return the corresponding node color based on a node's tag
	Vector getNodeColor(Node node)
	{
		if (node.tag == EMPTY)
		{
			if (node.hasParent())
			{
				if (node.settled) return Vector(1.0, 1.0, 0.0); // yellow
				else return Vector(0.0, 0.0, 1.0); // blue
			}
			else
			{
				return Vector(1.0, 1.0, 1.0); // white
			}
		}
		else if (node.tag == WALL) return Vector(0.1, 0.1, 0.1); // light grey
		else if (node.tag == SOURCE) return Vector(0.0, 1.0, 0.0); // green
		else if (node.tag == TARGET) return Vector(1.0, 0.0, 0.0); // red
	}

	void draw()
	{
		drawGrid();
		if (m_pathPoints != NULL) drawPath();
	}

	// draw cells in the grid in the order of left to right, top to bottom.
	void drawGrid()
	{
		int vIndex = 0, rectNo = 0;
		Vector vColor;
		for (int i = 0; i < m_row; i++)
		{
			for (int j = 0; j < m_col; j++)
			{
				vColor = getNodeColor(m_nodes[i*m_maxCol + j]);

				for (int i = 0; i < 18; i++)
				{
					m_colors[(rectNo * 18) + i] = vColor.mVal[i % 3];
				}

				vIndex = 0;
				m_vertices[(rectNo * 12) + vIndex] = m_posX + i * m_cellSize + i * m_cellGap; // x1
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posY + j * m_cellSize + j * m_cellGap; // y1
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posX + (i + 1) * m_cellSize + i * m_cellGap; // x2
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posY + j * m_cellSize + j * m_cellGap; // y2
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posX + i * m_cellSize + i * m_cellGap; // x3
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posY + (j + 1) * m_cellSize + j * m_cellGap; // y3
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_vertices[(rectNo * 12) + vIndex - 4]; // x2
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_vertices[(rectNo * 12) + vIndex - 4]; // y2
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_vertices[(rectNo * 12) + vIndex - 4]; // x3
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_vertices[(rectNo * 12) + vIndex - 4]; // y3
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posX + (i + 1) * m_cellSize + i * m_cellGap; // x4
				vIndex++;
				m_vertices[(rectNo * 12) + vIndex] = m_posY + (j + 1) * m_cellSize + j * m_cellGap; // y4
				rectNo++;
			}
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glVertexPointer(2, GL_FLOAT, 0, m_vertices);
		glColorPointer(3, GL_FLOAT, 0, m_colors);
		glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
	}

	// Called during initialization when the user dragged and moved the source node.
	void setSource(Node* node)
	{
		m_source = node;
		m_source->tag = SOURCE;
	}

	// called once when any of the searching algorithms is triggered.
	void preSearch()
	{
		// add source node to the list with its distance set to 0
		m_openList.clear();
		m_openList.push_back(m_source);
		m_source->distance = 0;
		m_source->gCost = 0;
	}

	// Called during initialization or when the user dragged and moved the target node.
	void setTarget(Node* node)
	{
		m_target = node;
		m_target->tag = TARGET;
	}

	// return true if the target is settled.
	bool dijkstra(bool allowDiagonal)
	{
		int curNodeIndex = findMinDistNodeIndex(m_openList);
		if (curNodeIndex == -1) // no node is found in the open list (there is no path)
			return true;
		m_curNode = &m_nodes[curNodeIndex]; // a node with the lowest distance value in open list 
		m_curNode->settled = true; // set the node as settled
		m_openList.remove(m_curNode); // remove the settled node from check list

		if (m_curNode->index == m_target->index) // path found
		{
			setPath();
			return true;
		}

		// evaluate all its accessible neighbors
		evaluateNeighbor(-1, 0); // left neighbor
		evaluateNeighbor(0, -1); // top neighbor
		evaluateNeighbor(0, 1); // bottom neighbor
		evaluateNeighbor(1, 0); // right neighbor

		if (allowDiagonal)
		{
			evaluateNeighbor(-1, -1); // top left
			evaluateNeighbor(-1, 1); // btm left
			evaluateNeighbor(1, 1); // btm right
			evaluateNeighbor(1, -1); // top right
		}
		return false;
	}

	// Used in Dijkstra algorithm. [dX, dY -> displacement (in cell unit) from the current node to the neighbor node.]
	void evaluateNeighbor(int dX, int dY)
	{
		Node* neighborNode = getNeighbor(m_curNode->index, dX, dY, m_maxRow, m_maxCol);
		if (neighborNode != NULL) // valid neighbor
		{
			if (neighborNode->isAccessible()) // not a wall & isn't settled
			{
				// calculate distance + compare + update distance & parent
				int distance = m_curNode->distance + int(10 * (abs(sqrtf(dX * dX + dY * dY))));
				if (neighborNode->distance >= distance)
				{
					neighborNode->distance = distance;
					neighborNode->parent = m_curNode;
				}
				if (!isListContainsNode(m_openList, *neighborNode))
				{
					m_openList.push_back(neighborNode);
				}
			}
		}
	}

	// best algorithm for one source & one target
	bool aStarSearch(bool allowDiagonal, distanceFunc heuristicFunc, int weight)
	{
		int curNodeIndex = findMinDistNodeIndex(m_openList); // select the node with the lowest f cost in the open list
		if (curNodeIndex == -1) // no node is found in the open list (there is no path)
			return true;
		m_curNode = &m_nodes[curNodeIndex];
		m_curNode->settled = true;
		m_openList.remove(m_curNode);

		if (curNodeIndex == m_target->index) // we have found the target
		{
			setPath();
			return true;
		}

		// evaluate all its accessible neighbors
		evaluateNeighbor(-1, 0, heuristicFunc, weight); // left neighbor
		evaluateNeighbor(0, -1, heuristicFunc, weight); // top neighbor
		evaluateNeighbor(0, 1, heuristicFunc, weight); // bottom neighbor
		evaluateNeighbor(1, 0, heuristicFunc, weight); // right neighbor

		if (allowDiagonal)
		{
			evaluateNeighbor(-1, -1, heuristicFunc, weight); // top left
			evaluateNeighbor(-1, 1, heuristicFunc, weight); // btm left
			evaluateNeighbor(1, 1, heuristicFunc, weight); // btm right
			evaluateNeighbor(1, -1, heuristicFunc, weight); // top right
		}

		return false;
	}

	// Used in AStar algorithm.
	void evaluateNeighbor(int dX, int dY, distanceFunc heuristicFunc, float weight)
	{
		Node* neighborNode = getNeighbor(m_curNode->index, dX, dY, m_maxRow, m_maxCol);
		
		if (neighborNode != NULL) // valid neighbor
		{
			if (neighborNode->isAccessible()) // not a wall & isn't settled
			{
				// calculate f (distance), g, h cost + compare + update distance & parent
				int thisNodeRow = neighborNode->index / m_maxCol;
				int thisNodeCol = neighborNode->index % m_maxCol;
				int targetRow = m_target->index / m_maxCol;
				int targetCol = m_target->index % m_maxCol;
				// H = the estimated movement cost to move from the current node on the grid to the target.
				int h = 10 * heuristicFunc(thisNodeRow, thisNodeCol, targetRow, targetCol);
				h *= weight;
				// G = the movement cost to move from the source to the current node on the grid, following the path generated to get there
				int g = m_curNode->gCost + int(10 * (abs(sqrtf(dX * dX + dY * dY))));

				if (!isListContainsNode(m_openList, *neighborNode))
				{
					m_openList.push_back(neighborNode);
				}

				if (neighborNode->gCost > g) // a smaller g cost means better path, record it!
				{
					neighborNode->gCost = g;
					neighborNode->distance = g + h;
					// cout << "gCost-> " << neighborNode->gCost << " ,hCost-> " << h << " ,fCost-> " << neighborNode->distance << endl;
					neighborNode->parent = m_curNode;
				}
			}
		}
	}

	Node* getNeighbor(int curNodeIndex, int offsetX, int offsetY, int totalRow, int totalCol)
	{
		int row = int(curNodeIndex / totalCol) + offsetX;
		int col = int(curNodeIndex % totalCol) + offsetY;

		// to disable grid wrapping
		if (row < 0 || row > totalRow - 1 || col < 0 || col > totalCol - 1) return NULL;
 
		return &m_nodes[row * totalCol + col];
	}

	// set all the nodes to their default stats
	void resetNodes()
	{
		int nodeIndex;
		for (int i = 0; i < m_maxRow; i++)
		{
			for (int j = 0; j < m_maxCol; j++)
			{
				nodeIndex = i * m_maxCol + j;
				m_nodes[nodeIndex].index = nodeIndex;
				m_nodes[nodeIndex].parent = NULL;
				m_nodes[nodeIndex].settled = false;
				m_nodes[nodeIndex].distance = INT_MAX;
				m_nodes[nodeIndex].gCost = INT_MAX;
			}
		}
	}

	void clearWalls()
	{
		for (int i = 0; i < m_maxRow; i++)
		{
			for (int j = 0; j < m_maxCol; j++)
			{
				if (m_nodes[(i * m_maxCol) + j].tag == WALL)
					m_nodes[(i * m_maxCol) + j].tag = EMPTY;
			}
		}
	}

	void clearPath()
	{
		delete[] m_pathPoints;
		m_pathPoints = NULL;
	}

	// store the points that form the path fron source node to target node.
	void setPath()
	{
		delete[] m_pathPoints;
		m_pathPointCount = 1; // +1 as source node is one of the path point eventho it doesn't has a parent
		Node* tempNode = m_curNode;
		while (tempNode->hasParent())
		{
			m_pathPointCount++;
			tempNode = tempNode->parent;
		}
		tempNode = m_curNode;
		m_pathPoints = new Vector[m_pathPointCount];
		for (int i = m_pathPointCount - 1; i >= 0; i--)
		{
			m_pathPoints[i] = calcCellPos(tempNode->index);
			tempNode = tempNode->parent;
		}
	}

	// draw connected lines from source node to target node
	void drawPath()
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glLineWidth(3.0f);
		for (int i = 0; i < m_pathPointCount - 1; i++)
		{
			glBegin(GL_LINES);
				glVertex2f(m_pathPoints[i].mVal[0], m_pathPoints[i].mVal[1]);
				glVertex2f(m_pathPoints[i + 1].mVal[0], m_pathPoints[i + 1].mVal[1]);
			glEnd();
		}
	}

	// Calculate and return the (centered) screen position of the cell 
	Vector calcCellPos(int cellIndex)
	{
		int row = cellIndex / m_maxCol;
		int col = cellIndex % m_maxCol;
		return Vector(m_posX + row * m_cellSize + (row - 1) * m_cellGap + (m_cellSize / 2), m_posY + col * m_cellSize + (col - 1) * m_cellGap + (m_cellSize / 2), 0);
	}

	bool isListContainsNode(list<Node*> data, Node node)
	{
		for (list<Node*>::iterator it = data.begin(); it != data.end(); ++it)
		{
			if ((*it)->index == node.index) return true;
		}
		return false;
	}

	// loop through the list and return the (first) node's index with the lowest distance value.
	// return -1 if no node is found.
	int findMinDistNodeIndex(list<Node*> data)
	{
		int index = -1, curIndex = -1;
		for (list<Node*>::iterator it = data.begin(); it != data.end(); ++it)
		{
			curIndex = (*it)->index;
			if (index == -1) // get the first element's index
			{
				index = curIndex;
			}
			else
			{
				if (m_nodes[curIndex].distance < m_nodes[index].distance) // found an element with a lower distance value
				{
					index = curIndex;
				}
			}
		}
		return index;
	}

	// Return the index of the cell which the mouse is hovering over.
	Node* getNodeHovered(float mouseX, float mouseY)
	{
		int rowClicked = floorf((mouseX - m_posX) / (m_cellSize + m_cellGap));
		// std::cout << "row clicked: " << rowClicked << std::endl;
		int colClicked = floorf((mouseY - m_posY) / (m_cellSize + m_cellGap));
		// std::cout << "col clicked: " << colClicked << std::endl;

		// ensure the node is in range
		if (rowClicked < 0 || rowClicked > m_maxRow - 1 || colClicked < 0 || colClicked > m_maxCol - 1) { return NULL; }

		return &m_nodes[rowClicked * m_maxCol + colClicked];
	}
};
#endif