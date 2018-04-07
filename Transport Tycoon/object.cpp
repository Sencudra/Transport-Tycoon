
#include <iostream>
#include <algorithm>
#include "object.h"
#include "player.h"


DynamicObject::DynamicObject(Player *player, Map* map, sf::Texture* texture, float x, float y)
{
    createObject(rs::ObjectType::VECHICALE, texture, x, y);
    m_map = map;
    m_player = player;

    m_cargoType = rs::Resources::COAL;
    m_capacity = 20;
    m_cargoLoaded = 0;

    m_finder = nullptr;
    m_path = nullptr;

    m_x = x;
    m_y = y;

    m_speedX = 0;
    m_speedY = 0;

    m_sprite.setTexture(*texture);
}

void DynamicObject::moveTaskSetup(rs::Point start, rs::Point end)
{
    findPath(start, end);

}

void DynamicObject::cargoExchange()
{
    Industries* obj = NULL;

    for(int x = m_x - 1; x < m_x + 1; ++x)
        for(int y = m_y - 1; y < m_y + 1; ++y)
        {

            if(m_map->m_map[x][y]->m_tileStatObj != NULL &&
               m_map->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::INDUSTRY )
            {
                obj = dynamic_cast<Industries*> (m_map->m_map[x][y]->m_tileStatObj);
                continue;
            }
        }

    if(obj != NULL && obj->getType() == rs::IndustryType::COALMINE)
    {
        //std::cout << "Available: " << obj->m_storage << std::endl;

        if(m_cargoLoaded != m_capacity && obj->m_storage != 0)
        {
            if(obj->m_storage < (m_capacity - m_cargoLoaded))
            {
                m_cargoLoaded += obj->m_storage;
                obj->m_storage = 0;
            }
            else
            {

                m_cargoLoaded += (m_capacity - m_cargoLoaded);
                obj->m_storage -= (m_capacity - m_cargoLoaded);
            }
        }
    }
    else if(obj != NULL && obj->getType() == rs::IndustryType::POWERSTATION)
    {
        std::cout << "Sold: " <<  (m_cargoLoaded) << std::endl;

        m_player->addMoney(m_cargoLoaded*100);
        m_cargoLoaded = 0;
    }
}

void DynamicObject::update(const float dt)
{

    if(m_path != nullptr && m_path->empty())
    {
        if(m_moveTask.front().x == m_x && m_moveTask.front().y == m_y)
        {
            delete m_finder;
            m_moveTask.push_back(*m_moveTask.begin());
            m_moveTask.erase(m_moveTask.begin());
            findPath(m_moveTask.back(), m_moveTask.front());
            cargoExchange();
        }
        m_speedX = 0;
        m_speedY = 0;
    }
    if(m_path != nullptr && !m_path->empty())
    {
        if(m_path->front()->x > m_x - 0.05 && m_path->front()->x < m_x + 0.05 &&
           m_path->front()->y > m_y - 0.05 && m_path->front()->y < m_y + 0.05)
        {
            m_x = m_path->front()->x;
            m_y = m_path->front()->y;

            //std::cout << m_x << " " << m_y << std::endl;

            m_path->erase(m_path->begin());
            if(m_path != nullptr)
            {
                float x,y;
                x = m_path->front()->x;
                y =  m_path->front()->y;

                m_speedX = x - m_x;
                m_speedY = y - m_y;
            }
        }
    }

    float x,y;

    m_x += (m_speedX*dt);
    m_y += (m_speedY*dt);

    x = m_x;
    y = m_y;

    rs::twoDToIso(x,y,64,32);
    m_sprite.setPosition(x, y);


}

DynamicObject::~DynamicObject()
{
    delete m_finder;
}

void DynamicObject::draw(sf::RenderWindow *view)
{
    float x,y;
    x = m_x;
    y = m_y;
    rs::twoDToIso(x,y, 64, 32);
    m_sprite.setPosition(x+27,y+11); // TO GET TO THE CENTER
    view->draw(m_sprite);
}

void DynamicObject::findPath(rs::Point start, rs::Point end)
{
    m_finder = new pathFinderA(m_map, start, end);
    m_path = m_finder->getPath();
}

void DynamicObject::addTask(rs::Point task)
{
        m_moveTask.push_back(task);

        if(!m_isActive)
        {
            m_isActive = true;

            rs::Point start, end;
            start.setValues(m_x, m_y);
            end.setValues(m_moveTask.front().x,m_moveTask.front().y);
            moveTaskSetup(start, end);
        }
}


Road::Road(rs::ObjectType objType, sf::Texture* texture, rs::RoadType type)
{
    createObject(objType, texture,0,0);

    m_type = type;
    m_sprite.setTexture(*texture);

}

void Road::update(const float dt)
{

}

void Road::draw(sf::RenderWindow *view)
{
    view->draw(this->m_sprite);
}

void Road::updateSprite(sf::Texture *texture)
{
    m_sprite.setTexture(*texture);
}


Industries::Industries(rs::ObjectType objType, sf::Texture* texture, rs::IndustryType type, float x, float y)
{
    createObject(objType,texture,x,y);

    /* different objects have different size and sprite size. */
    int pointMapping;
    switch (type) {
    case rs::IndustryType::COALMINE:
        pointMapping = 64;
        break;
    case rs::IndustryType::POWERSTATION:
        pointMapping = 48;
        break;
    default:
        break;
    }

    m_storage = 0;

    m_type = type;

    m_sprite.setPosition(sf::Vector2f(x, y-(m_texture->getSize().y)+pointMapping));
    m_sprite.setTexture(*texture);

    m_isActive = true;
    setProp(type);
}

void Industries::update(const float dt)
{
    if(m_isActive)
    {
        m_storage+= dt/5*m_workSpeed;
    }
}

void Industries::draw(sf::RenderWindow* view)
{

    view->draw(this->m_sprite);
}

void Industries::setProp(const rs::IndustryType type)
{
    srand(time(NULL));

    switch (type) {

    using namespace rs;

    case IndustryType::COALMINE:
    {
        m_iResNum = 0;
        m_oResNum = 1;
        m_oRsrc[0] = Resources::COAL;

        m_isActive = true;

        m_workSpeed = 100;

    }break;
    case IndustryType::POWERSTATION:
    {
        m_iResNum = 1;
        m_oResNum = 0;

        m_iRsrc[0] = Resources::COAL;
        m_workSpeed = 1000;

    }break;
    case IndustryType::FACTORY:
    {
        m_iResNum = 1;
        m_oResNum = 0;

        m_iRsrc[0] = Resources::COAL;
        m_workSpeed = 1000;

    }break;
    default:
    {
        m_workSpeed = 0;
    }break;
    }
}

void Industries::setIsActive()
{
    m_isActive = true;
}


pathFinderA::pathFinderA(Map* map, rs::Point start, rs::Point goal)
{
    m_tileMap = map;

    rs::PPoint* first = new rs::PPoint();
    first->x = start.x;
    first->y = start.y;

    first->g = 0;
    first->h = heuristic_cost(first, goal);
    first->f = first->g + first->h;

    first->cameFrom = nullptr;

    m_start = first;
    m_openSet.push_back(first);

    m_goal.setValues(goal.x, goal.y);

    findPath();
}

bool pathFinderA::findPath(){

    while(!m_openSet.empty())
    {
        float min = m_openSet.front()->f;
        rs::PPoint* vertex = m_openSet.front();
        int num = 0;
        int counter = 0;

        for(rs::PPoint* i : m_openSet)
        {
            if(i->f < min){
                min = i->f;
                vertex = i;
                num = counter;
            }
            counter++;
        }

        if(vertex->x == m_goal.x && vertex->y == m_goal.y)
        {
            reconstructPath(vertex);
            return true;
        }


        m_openSet.erase(m_openSet.begin()+num);
        m_closedSet.push_back(vertex);

        checkNeighbTiles(vertex);

    }
    return false;
}

void pathFinderA::checkNeighbTiles(rs::PPoint* vertex)
{
    int mapSize = m_tileMap->getMapSize();

    int mask[4][2] = {{-1,0},{0, -1},{1, 0},{0, 1}};

    for(int i = 0; i < 4; ++i)
    {
        int x = vertex->x + mask[i][0];
        int y = vertex->y + mask[i][1];

        if(x < mapSize && x > 0 && y < mapSize && y > 0 &&
                m_tileMap->m_map[x][y]->m_tileStatObj != NULL &&
                m_tileMap->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
        {

            bool isInClosedSet = vertInSet(m_closedSet, x, y);
            bool tentativeIsBetter = false;

            if(!isInClosedSet)
            {
                float tentative_g_score = vertex->g + 1; // distance between x and y (neighbours)
                rs::PPoint* newVertex;

                bool isInOpenSet = vertInSet(m_openSet, x, y);

                if(!isInOpenSet)
                {
                    newVertex = new rs::PPoint();
                    newVertex->x = x;
                    newVertex->y = y;
                    m_openSet.push_back(newVertex);
                    tentativeIsBetter = true;
                }
                else
                {
                    if(tentative_g_score < newVertex->g)
                    {
                        tentativeIsBetter = true;
                    }
                    else
                    {
                        tentativeIsBetter = false;
                    }
                }
                if(tentativeIsBetter == true)
                {
                    newVertex->cameFrom = vertex;
                    newVertex->g = tentative_g_score;
                    newVertex->h = heuristic_cost(newVertex, m_goal);
                    newVertex->f = newVertex->g + newVertex->h;
                }
            }
        }
    }
}


bool pathFinderA::vertInSet(const std::deque<rs::PPoint*> set, int x, int y)
{
    for (auto iterator = set.begin(); iterator != set.end(); ++iterator)
    {
        if ((*iterator)->x == x && (*iterator)->y == y)
            return true;
    }
    return false;
}

void pathFinderA::reconstructPath(rs::PPoint* goal)
{
    rs::PPoint* currentNode = goal;
    while (currentNode != nullptr)
    {
        m_pathMap.insert(m_pathMap.begin(),currentNode);
        currentNode = currentNode->cameFrom;
    }
}

float pathFinderA::heuristic_cost(rs::PPoint *start, rs::Point goal)
{
    return sqrt(pow(abs(start->x - goal.x),2)+pow(abs(start->y - goal.y),2));
}
