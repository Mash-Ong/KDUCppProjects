#ifndef _NODE_H
#define _NODE_H

enum NodeTag
{
	EMPTY = 0,
	WALL = 1,
	SOURCE = 2,
	TARGET = 3
};

class Node
{
public:
	int index; // index of itself in the grid's nodes array
	Node* parent = NULL;
	NodeTag tag = EMPTY;
	bool settled = false;
	int distance = INT_MAX; // f cost, in the context of A* algorithm
	int gCost = INT_MAX; // G = the movement cost to move from the source to the current node on the grid, following the path generated to get there

	Node() {}

	bool hasParent() { return !(parent == NULL); }
	bool isAccessible() { return !(tag == WALL) && !settled; }
};
#endif