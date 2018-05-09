
#include <iostream>
#include <algorithm>
#include "object.h"
#include "player.h"
#include "Pathfinder.h"
#include "texturemanager.h"
#include "resources.h"


int Object::createObject(rs::ObjectType type, sf::Texture* texture, float x, float y)
{
	m_objectType = type;
	m_texture = texture;
	m_x = x;
	m_y = y;

	m_isSelected = false;
	
	return 0;
}



DynamicObject::~DynamicObject()
{
	delete m_finder;
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

void DynamicObject::addTask(rs::Point task)
{
	m_moveTask.push_back(task);

	if (!m_isActive)
	{
		m_isActive = true;

		rs::Point start, end;
		start.setValues(m_x, m_y);
		end.setValues(m_moveTask.front().x, m_moveTask.front().y);
		moveTaskSetup(start, end);
	}
}


Vehicle::Vehicle(rs::vhs::Vehicle vehStruct, Player * player, Map * map,sf::Texture * texture, float x, float y)
{
	m_player = player;
	m_map = map;
	createObject(rs::ObjectType::VEHICLE, texture, x, y);

	m_finder = new PathFinder(map);

	m_x_iso = m_x = x;
	m_y_iso = m_y = y;

	m_speedX = 0;
	m_speedY = 0;

	m_sprite.setTexture(*texture);

	//m_cargoType = rs::Resources::COAL;
	//m_capacity = 20;
	//m_cargoLoaded = 0;

}

Vehicle::~Vehicle()
{
}

void Vehicle::update(const float dt)
{

	if (m_path != nullptr && m_path->empty())
	{
		if (m_moveTask.front().x == this->m_x && m_moveTask.front().y == this->m_y)
		{
			std::cout << "DynamicObject::update: Task DONE" << std::endl;
			m_moveTask.push_back(*m_moveTask.begin());
			m_moveTask.erase(m_moveTask.begin());
			moveTaskSetup(m_moveTask.back(), m_moveTask.front());
			//cargoExchange();
		}
		m_speedX = 0;
		m_speedY = 0;
	}
	if (m_path != nullptr && !m_path->empty())
	{
		if (m_path->front()->x > m_x_iso - 0.05 && m_path->front()->x < m_x_iso + 0.05 &&
			m_path->front()->y > m_y_iso - 0.05 && m_path->front()->y < m_y_iso + 0.05)
		{
			m_x_iso = m_x = m_path->front()->x;
			m_y_iso = m_y = m_path->front()->y;

			m_path->erase(m_path->begin());

			if (m_path->size() != 0 && m_path != nullptr)
			{
				int x, y;
				x = m_path->front()->x;
				y = m_path->front()->y;

				m_speedX = x - m_x;
				m_speedY = y - m_y;
			}
		}
	}

	m_x_iso += (m_speedX*dt);
	m_y_iso += (m_speedY*dt);

	// Changing object's tile
	if (m_x != m_x_iso || m_y != m_y_iso)
	{
		int counter = 0;
		bool isFound = false;
		for (Object* i : this->m_map->m_map[m_x][m_y]->m_tileDynObj)
		{
			if (i == this)
			{
				this->m_map->m_map[m_x][m_y]->m_tileDynObj.erase(this->m_map->m_map[m_x][m_y]->m_tileDynObj.begin() + counter);
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
			m_x = round(m_x_iso);
			m_y = round(m_y_iso);
			this->m_map->m_map[m_x][m_y]->m_tileDynObj.push_back(this);
		}

	}
}

void Vehicle::draw(sf::RenderWindow& view)
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

	float x, y;
	x = m_x_iso;
	y = m_y_iso;
	rs::twoDToIso(x, y, 64, 32);
	m_sprite.setPosition(x + 27, y + 11); // TO GET TO THE CENTER
	view.draw(m_sprite);
}

void Vehicle::loadObject(sf::Texture * texture, Map * map, Player * player)
{
	m_texture = texture;
	m_map = map;
	m_player = player;

	m_finder = new PathFinder(m_map);
	if (!m_moveTask.empty())
	{
		m_isActive = true;

		rs::Point start, end;
		start.setValues(m_x, m_y);
		end.setValues(m_moveTask.front().x, m_moveTask.front().y);
		moveTaskSetup(start, end);
	}

	m_sprite.setTexture(*texture);
	m_map->m_map[m_x][m_y]->m_tileDynObj.push_back(this);

}

void Vehicle::cargoExchange()
{
    Industry* obj = NULL;

    for(int x = m_x - 1; x < m_x + 1; ++x)
        for(int y = m_y - 1; y < m_y + 1; ++y)
        {

            if(m_map->m_map[x][y]->m_tileStatObj != NULL &&
               m_map->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::INDUSTRY )
            {
                obj = dynamic_cast<Industry*> (m_map->m_map[x][y]->m_tileStatObj);
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



Road::Road(rs::ObjectType objType, sf::Texture* texture, rs::RoadType type, int x, int y)
{
    createObject(objType, texture, x, y);

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

void Road::loadObject(sf::Texture * texture)
{
	m_texture = texture;
	updateSprite(texture);
}

void Road::updateSprite(sf::Texture *texture)
{
    m_sprite.setTexture(*texture);
}



Industry::Industry(rs::ObjectType objType, sf::Texture* texture, rs::IndustryType type, float x, float y)
{
    createObject(objType,texture,x,y);

	m_type = type;
	m_storage = 0;
	m_isActive = true;
	setProp(type);
	loadObject(texture);
    
}

void Industry::update(const float dt)
{
    if(m_isActive)
    {
        m_storage+= int(dt/5*m_workSpeed);
    }
}

void Industry::draw(sf::RenderWindow& view)
{
    view.draw(this->m_sprite);
}

void Industry::loadObject(sf::Texture * texture)
{ // Loads sprite settings
	m_texture = texture;

	/* different objects have different sprite size. */
	int pointMapping;
	switch (this->m_type) {
	case rs::IndustryType::COALMINE:
		pointMapping = 64;
		break;
	case rs::IndustryType::POWERSTATION:
		pointMapping = 48;
		break;
	default:
		break;
	}

	float x1, y1;
	x1 = this->m_x;
	y1 = this->m_y;
	rs::twoDToIso(x1, y1, 64, 32);

	m_sprite.setTexture(*m_texture);
	m_sprite.setPosition(sf::Vector2f(x1, y1 - (m_texture->getSize().y) + pointMapping));


}

void Industry::setProp(const rs::IndustryType type)
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

void Industry::setIsActive()
{
    m_isActive = true;
}


