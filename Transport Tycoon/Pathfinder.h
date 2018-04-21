#pragma once

#include "resources.h"
#include <math.h>


class Map;

struct PPoint   // Point struct for path finding algorithm
{
	int x, y;    //  Position
	float g;    //  Cost of the path from the start point
	float h;    //  Heuristic cost estimate
	float f;    //  Sum of H and F

	PPoint* cameFrom;
};

class PathFinder
{	// Path finding algorithm
public:
	PathFinder(Map *map);
	PathFinder(Map *map, rs::Point start, rs::Point goal);
	~PathFinder()
	{
		for (auto i : m_closedSet)
			delete i;
	}

	int setupPath(rs::Point start, rs::Point end);

	std::vector<PPoint*>* getPath() { return &m_pathMap; }

	bool findPath();

private:
	
	
	void checkNeighbTiles(PPoint* vertex);
	void reconstructPath(PPoint *goal);
	float heuristic_cost(PPoint* start, rs::Point end);
	
	PPoint* vertInSet(const std::deque<PPoint*> set, int x, int y);

private:

	bool isReady; // is ready to find path

	Map* m_tileMap; // Map image save
	PPoint* m_start;
	rs::Point m_end;

	std::deque<PPoint*>  m_closedSet;   // Visited points
	std::deque<PPoint*>   m_openSet;     // Available points to check in the future

public:
	std::vector<PPoint*>  m_pathMap;     // Path from start to goal

};

