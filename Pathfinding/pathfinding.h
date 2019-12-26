#ifndef _PATHFINDING_H
#define _PATHFINDING_H

#include "demo_base.h"
#include "node.h"
#include "grid.h"
#include "button.h"
#include "bitmap.h"
#include <time.h>

enum Algorithm
{
	DIJKSTRA = 0,
	ASTAR = 1
};

class PathFinding : public DemoBase
{
	bool m_allowDiagonal = false;
	bool m_pathFinding = false; // set to true when "play" button is clicked.
	Algorithm algorithm = DIJKSTRA;
	// input
	bool m_leftBtnDown = false;
	double m_mouseX = 0.0;
	double m_mouseY = 0.0;
	int m_windowWidth, m_windowHeight;
	Node* m_prevNodeHovered = NULL;
	Node* m_nodeDragged = NULL;
	Grid* m_grid = NULL;

public:
	float aStarWeight = 1.0f;

	PathFinding(int windowW, int windowH)
	{
		m_windowWidth = windowW;
		m_windowHeight = windowH;
	}

	void init()
	{
		m_grid = new Grid(5, 5, m_windowWidth - 125, m_windowHeight - 5);
		m_grid->initGrid();
	}

	void deinit()
	{
		m_grid->deinit();
	}
	
	void draw()
	{
		m_grid->draw();
		if (m_pathFinding)
		{
			switch (algorithm)
			{
			case DIJKSTRA:
				m_pathFinding = !m_grid->dijkstra(m_allowDiagonal);
				break;

			case ASTAR:
				m_pathFinding = !m_grid->aStarSearch(m_allowDiagonal, euclideanDistance, aStarWeight);
				break;

			default:
				break;
			}
		}
	}

	void clearWalls()
	{
		if (!m_pathFinding)
		{
			m_grid->clearWalls();
		}
	}

	void clearGrid()
	{
		m_grid->clearPath();
		m_grid->resetNodes();
	}

	void startSearching()
	{
		m_pathFinding = true;
		m_grid->preSearch();
	}

	void onMouseButton(int button, int action)
	{
		if (button == 0)
		{
			m_leftBtnDown = action == 1 ? true : false;
			if (m_leftBtnDown && !m_pathFinding)
			{
				Node* nodeHovering = m_grid->getNodeHovered(m_mouseX, m_mouseY);
				if (nodeHovering)
					if (nodeHovering->tag == SOURCE || nodeHovering->tag == TARGET) // source and target node can be repositioned by dragging them
					{
						m_nodeDragged = nodeHovering;
					}
					else if (nodeHovering->tag == EMPTY) nodeHovering->tag = WALL;
					else if (nodeHovering->tag == WALL) nodeHovering->tag = EMPTY;
			}
			else
			{
				m_nodeDragged = NULL;
			}
		}

	}

	void onMouseMove(double x, double y)
	{
		m_mouseX = x;
		m_mouseY = y;

		if (m_leftBtnDown && !m_pathFinding) 
		{
			Node* nodeHovering = m_grid->getNodeHovered(m_mouseX, m_mouseY);
			if (nodeHovering && nodeHovering != m_prevNodeHovered)
			{
				if (nodeHovering->tag == EMPTY)
				{
					if (m_nodeDragged) // is the user dragging source or target node around? -> replace source/target node
					{
						if (m_nodeDragged->tag == SOURCE)
							m_grid->setSource(nodeHovering);
						else if (m_nodeDragged->tag == TARGET)
							m_grid->setTarget(nodeHovering);
						m_nodeDragged->tag = EMPTY;
						m_nodeDragged = nodeHovering;
					}
					else
						nodeHovering->tag = WALL; // switch empty node to wall (obstacle)
				}					
				else if (nodeHovering->tag == WALL && !m_nodeDragged) nodeHovering->tag = EMPTY;
				m_prevNodeHovered = nodeHovering;
			}
		}
	}

	void toggleAlgorithm()
	{
		if (algorithm == ASTAR)
			algorithm = DIJKSTRA;
		else
			algorithm = ASTAR;

		if (m_pathFinding)
		{
			clearGrid();
			startSearching();
		}
	}

	void toggleDiagonalSearch()
	{
		if (!m_pathFinding)
		{
			m_allowDiagonal = !m_allowDiagonal;
		}
	}
};
#endif // !_PATHFINDING_H

