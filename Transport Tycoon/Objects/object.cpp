
#include <iostream>
#include <algorithm>
#include "object.h"
#include "player.h"
#include "Pathfinder.h"
#include "resources.h"


DynamicObject::DynamicObject(Player *player, Map* map, sf::Texture* texture, float x, float y)
{

	m_map = map;
    createObject(rs::ObjectType::VECHICALE, texture, x, y);
    m_player = player;

    m_cargoType = rs::Resources::COAL;
    m_capacity = 20;
    m_cargoLoaded = 0;

    m_finder = new PathFinder(map);

    m_x = x;
    m_y = y;

    m_speedX = 0;
    m_speedY = 0;

    m_sprite.setTexture(*texture);
}

void DynamicObject::moveTaskSetup(rs::Point start, rs::Point end)
{
	if (m_finder)
	{
		if (m_finder->setupPath(start, end))
		{
			std::cout << "ERROR DynamicObject::moveTaskSetup: setupPath()" << std::endl;
			
		}
		else
		{
			if (m_finder->findPath())
			{
				m_path = m_finder->getPath();
			}
		}
	}
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
        if(m_moveTask.front().x == this->x && m_moveTask.front().y == this->y)
        {
			//std::cout << "Task DONE" << std::endl;
            m_moveTask.push_back(*m_moveTask.begin());
            m_moveTask.erase(m_moveTask.begin());
            moveTaskSetup(m_moveTask.back(), m_moveTask.front());
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


            m_path->erase(m_path->begin());
            if(m_path->size() != 0 && m_path != nullptr)
            {
                float x,y;
                x = m_path->front()->x;
                y =  m_path->front()->y;

                m_speedX = x - m_x;
                m_speedY = y - m_y;
            }
        }
    }

	m_x += (m_speedX*dt);
	m_y += (m_speedY*dt);
	
	// Changing object's tile
	if (round(m_x) != this->x ||
		round(m_y) != this->y)
	{
		int counter = 0;
		bool isFound = false;
		for (Object* i : this->m_map->m_map[x][y]->m_tileDynObj)
		{
			if (i == this)
			{
				this->m_map->m_map[x][y]->m_tileDynObj.erase(this->m_map->m_map[x][y]->m_tileDynObj.begin() + counter);
				isFound = true;
			}
			counter++;
		}
		if (!isFound)
		{
			std::cout << "ERROR DynamicObject::update: Object not found on tile" << std::endl;
		}
		else
		{
			this->x = round(m_x);
			this->y = round(m_y);
			this->m_map->m_map[x][y]->m_tileDynObj.push_back(this);
		}

	}
}

DynamicObject::~DynamicObject()
{
    delete m_finder;
}

void DynamicObject::draw(sf::RenderWindow& view)
{
	if (m_path != nullptr && !m_path->empty())
	{	// Draw next tile and previous tile

		float nextTileX = m_path->front()->x;
		float nextTileY = m_path->front()->y;

		rs::twoDToIso(nextTileX, nextTileY, 64, 32);

		m_map->m_map[m_path->front()->x][m_path->front()->y]->partDraw(nextTileX, nextTileY, view);

		if (m_path->front()->cameFrom != nullptr)
		{
			int x1, y1;
			x1 = m_path->front()->cameFrom->x;
			y1 = m_path->front()->cameFrom->y;
			float prevTileX = m_path->front()->cameFrom->x;
			float prevTileY = m_path->front()->cameFrom->y;
			rs::twoDToIso(prevTileX, prevTileY, 64, 32);

			m_map->m_map[x1][y1]->partDraw(prevTileX, prevTileY, view);
		}
	}

    float x,y;
    x = m_x;
    y = m_y;
    rs::twoDToIso(x,y, 64, 32);
    m_sprite.setPosition(x+27,y+11); // TO GET TO THE CENTER
    view.draw(m_sprite);
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

void Road::draw(sf::RenderWindow& view)
{
    view.draw(this->m_sprite);
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
        m_storage+= int(dt/5*m_workSpeed);
    }
}

void Industries::draw(sf::RenderWindow& view)
{

    view.draw(this->m_sprite);
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


