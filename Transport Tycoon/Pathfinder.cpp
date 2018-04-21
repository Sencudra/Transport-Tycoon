
#include "resources.h"
#include "map.h"
#include "object.h"
#include "Pathfinder.h"


PathFinder::PathFinder(Map* map, rs::Point start, rs::Point end)
{
	this->m_tileMap = map;
	this->isReady = false;
	this->setupPath(start, end);
}

PathFinder::PathFinder(Map *map)
{
	this->m_tileMap = map;
	this->isReady = false;
}

int PathFinder::setupPath(rs::Point start, rs::Point end)
{	// Setup pathfinder

	PPoint* firstPoint = new PPoint();
	firstPoint->x = start.x;
	firstPoint->y = start.y;

	firstPoint->g = 0;
	firstPoint->h = heuristic_cost(firstPoint, end);
	firstPoint->f = firstPoint->g + firstPoint->h;
	firstPoint->cameFrom = nullptr;


	m_closedSet.clear();
	m_openSet.clear();
	m_pathMap.clear();

	m_openSet.push_back(firstPoint);

	// Save end points
	m_start = firstPoint;
	m_end.setValues(end.x, end.y);
	isReady = true;

	std::cout << "PathFinder::setupPath: setup complete" << std::endl;

	return 0;
}

bool PathFinder::findPath() {

	if (!isReady)
	{
		std::cout << "PathFinder::findPath: isReady = false" << std::endl;
		return false;
	}

	while (!m_openSet.empty())
	{ 

		PPoint* vertex = m_openSet.front();
		
		float min = vertex->f;		
		
		int num = 0;
		int counter = 0;

		for (PPoint* i : m_openSet)
		{ // lowest f score finding
			if (i->f < min) {
				min = i->f;
				vertex = i;
				num = counter; // save point position
			}
			counter++;
		}

		if (vertex->x == m_end.x && vertex->y == m_end.y)
		{	//End is found
			reconstructPath(vertex);
			return true;
		}

		m_openSet.erase(m_openSet.begin() + num);
		m_closedSet.push_back(vertex);

		checkNeighbTiles(vertex);

	}
	return false;
}

void PathFinder::checkNeighbTiles(PPoint* vertex)
{
	int mapSize = m_tileMap->getMapSize();
	int mask[4][2] = { { -1,0 },{ 0, -1 },{ 1, 0 },{ 0, 1 } };
	m_tileMap->m_map;

	for (int i = 0; i < 4; ++i)
	{
		int x = vertex->x + mask[i][0];
		int y = vertex->y + mask[i][1];

		if (x < mapSize && x > 0 && y < mapSize && y > 0 &&		
			m_tileMap->m_map[x][y]->m_tileStatObj != NULL &&
			m_tileMap->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
		{

			PPoint* isInClosedSet = vertInSet(m_closedSet, x, y);
			bool tentativeIsBetter = false;

			if (!isInClosedSet)
			{
				float tentative_g_score = vertex->g + 1; // distance between x and y (neighbours)
				
				PPoint* newVertex = vertInSet(m_openSet, x, y);

				if (newVertex == nullptr)
				{
					newVertex = new PPoint();
					newVertex->x = x;
					newVertex->y = y;
					m_openSet.push_back(newVertex);
					tentativeIsBetter = true;
				}
				else
				{
					if (tentative_g_score < newVertex->g)
					{
						tentativeIsBetter = true;
					}
					else
					{
						tentativeIsBetter = false;
					}
				}
				if (tentativeIsBetter == true)
				{
					newVertex->cameFrom = vertex;
					newVertex->g = tentative_g_score;
					newVertex->h = heuristic_cost(newVertex, m_end);
					newVertex->f = newVertex->g + newVertex->h;
				}
			}
		}
	}
}

PPoint* PathFinder::vertInSet(const std::deque<PPoint*> set, int x, int y)
{
	for (auto iterator = set.begin(); iterator != set.end(); ++iterator)
	{
		if ((*iterator)->x == x && (*iterator)->y == y)
			return (*iterator);
	}
	return nullptr;
}

void PathFinder::reconstructPath(PPoint* goal)
{
	if (!goal)
	{
		std::cout << "PathFinder::reconstructPath: Nothing to reconstruct" << std::endl;
	}

	PPoint* currentNode = goal;
	while (currentNode != nullptr)
	{
		m_pathMap.insert(m_pathMap.begin(), currentNode);
		currentNode = currentNode->cameFrom;
	}
	std::cout << "PathFinder::reconstructPath: Path created" << std::endl;
	return;
}

float PathFinder::heuristic_cost(PPoint *start, rs::Point goal)
{
	return sqrt(pow(abs(start->x - goal.x), 2) + pow(abs(start->y - goal.y), 2));
}