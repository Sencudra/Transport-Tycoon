
#include "resources.h"
#include <iostream>
#include <algorithm>
#include "object.h"
#include "player.h"
#include "Pathfinder.h"
#include "texturemanager.h"
#include <random>



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
	if (m_map->m_map[task.x][task.y]->m_tileStatObj == nullptr ||
		m_map->m_map[task.x][task.y]->m_tileStatObj->m_objectType != rs::ObjectType::ROAD ) return;
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

Vehicle::Vehicle(vhs::Vehicle vehStruct, Map* map, float x, float y)
{
	m_map = map;
	m_vehicleInfo = vehStruct;
	m_finder = new PathFinder(map);
	m_direction = vhs::Directions::BOTTOM_RIGHT;

	createObject(rs::ObjectType::VEHICLE, &m_vehicleInfo.sprites->at(m_direction), x, y);
	m_sprite.setTexture(*m_texture);

	m_x_iso = m_x = x;
	m_y_iso = m_y = y;

	m_speedX = 0;
	m_speedY = 0;

	m_cargoLoaded = 0;
	m_isBroken = false;

	
}

Vehicle::~Vehicle()
{
}

void Vehicle::update(const float dt)
{
	// Path follower
	if (m_path != nullptr && m_path->empty())
	{
		if (m_moveTask.front().x == this->m_x && m_moveTask.front().y == this->m_y)
		{
			std::cout << "DynamicObject::update: Task DONE" << std::endl;
			m_moveTask.push_back(*m_moveTask.begin());
			m_moveTask.erase(m_moveTask.begin());
			moveTaskSetup(m_moveTask.back(), m_moveTask.front());
			cargoExchange();
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

				m_speedX = (x - m_x);
				m_speedY = (y - m_y);

				updateDirection();
			}
		}
	}

	// Moving Object
	m_x_iso += (m_speedX*dt)*m_vehicleInfo.speed / 100;
	m_y_iso += (m_speedY*dt)*m_vehicleInfo.speed / 100;

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

void Vehicle::loadObject(vhs::Vehicle vehStruct, Map* map)
{
	m_x_iso = m_x;
	m_y_iso = m_y;
	m_map = map;

	// Temopary save
	int id = vehStruct.id;
	Player* player = vehStruct.owner;
	m_vehicleInfo = vehStruct;
	m_vehicleInfo.id = id;
	m_vehicleInfo.owner = player;

	m_texture = &m_vehicleInfo.sprites->at(m_direction);
	m_sprite.setTexture(*m_texture);

	m_finder = new PathFinder(m_map);
	if (!m_moveTask.empty())
	{
		m_isActive = true;

		rs::Point start, end;
		start.setValues(m_x, m_y);
		end.setValues(m_moveTask.front().x, m_moveTask.front().y);
		moveTaskSetup(start, end);
	}
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
                break;
            }
        }
	if (obj == NULL) return;


    if( obj->getType() == rs::IndustryType::COALMINE)
    {
        //std::cout << "Available: " << obj->m_storage << std::endl;
		load(obj);

    }
	else if (obj->getType() == rs::IndustryType::POWERSTATION)
	{
		unload(obj);
	}
	else if (obj->getType() == rs::IndustryType::IRONOREMINE)
	{
		load(obj);
	}
	else if (obj->getType() == rs::IndustryType::STEELMILL)
	{
		unload(obj);
	}
	else if (obj->getType() == rs::IndustryType::FARM)
	{
		load(obj);
	}
	else if (obj->getType() == rs::IndustryType::FACTORY)
	{
		load(obj);
	}
	else if (obj->getType() == rs::IndustryType::FOREST)
	{
		load(obj);
	}
	else if (obj->getType() == rs::IndustryType::SAWMILL)
	{
		unload(obj);
	}

}

void Vehicle::unload(Industry* obj)
{
	std::cout << "Sold: " << (m_cargoLoaded) << std::endl;
	obj->m_storage += m_cargoLoaded;

	srand(time(NULL));
	int rate = (rand() % 50) + (rand() % 25 + 10);
	m_vehicleInfo.owner->addMoney(m_cargoLoaded * rate);

	m_cargoLoaded = 0;

}

void Vehicle::load(Industry * obj)
{
	if (m_cargoLoaded != m_vehicleInfo.capacity && obj->m_storage != 0)
	{
		if (obj->m_storage < (m_vehicleInfo.capacity - m_cargoLoaded))
		{
			m_cargoLoaded += obj->m_storage;
			obj->m_storage = 0;
		}
		else
		{
			m_cargoLoaded += (m_vehicleInfo.capacity - m_cargoLoaded);
			obj->m_storage -= (m_vehicleInfo.capacity);
		}
	}

}

void Vehicle::updateDirection()
{

	if (m_speedX < 0 && m_speedY == 0)	m_direction = vhs::Directions::TOP_LEFT;
	else if(m_speedX == 0 && m_speedY > 0)	m_direction = vhs::Directions::BOTTOM_LEFT;
	else if(m_speedX > 0 && m_speedY == 0)	m_direction = vhs::Directions::BOTTOM_RIGHT;
	else if(m_speedX == 0 && m_speedY < 0)	m_direction = vhs::Directions::TOP_RIGHT;
	else std::cout << "ERROR: Vehicle::updateDirection" << std::endl;


	float x, y;
	x = m_x_iso;
	y = m_y_iso;
	rs::twoDToIso(x, y, 64, 32);
	m_sprite.setTexture(m_vehicleInfo.sprites->at(m_direction));
	m_sprite.setPosition(x + 27, y + 11); // TO GET TO THE CENTER

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
	m_time_elapsed = 0;
	m_isActive = true;
	loadObject(texture);
    
}

void Industry::update(const float dt)
{
	m_time_elapsed += dt;
    if(m_isActive && m_time_elapsed > 2.5)
    {
		if (m_iResNum > 0)
		{
			if (m_storage - m_workSpeed > 0)
			{
				m_storage -= int(m_workSpeed);
				m_time_elapsed = 0;
			}
			else
			{
				m_storage = 0 ;
				m_time_elapsed = 0;
			}

		}
		else if (m_oResNum > 0)
		{
			m_storage += int(m_workSpeed);
			m_time_elapsed = 0;
		}

    }
}

void Industry::draw(sf::RenderWindow& view)
{
    view.draw(this->m_sprite);
}

void Industry::loadObject(sf::Texture * texture)
{ // Loads sprite settings
	setProp(m_type);
	m_texture = texture;
	m_time_elapsed = 0;
	

	/* different objects have different sprite size. */
	int pointMapping, xPoint;
	switch (this->m_type) {
	case rs::IndustryType::COALMINE:
	{
		pointMapping = 64;
		xPoint = 0;
	}	
	break;
	case rs::IndustryType::POWERSTATION:
	{
		pointMapping = 48;
		xPoint = 0;
	}
	break;
	case rs::IndustryType::BANK :
	{
		pointMapping = 32;
		xPoint = 0;
	}
	break;
	case rs::IndustryType::FACTORY :
	{
		pointMapping = 64;
		xPoint = 0;
	}	
	break;
	case rs::IndustryType::FARM :
	{
		pointMapping = 82;
		xPoint = -6;
	}
	break;
	case rs::IndustryType::FOREST :
	{
		pointMapping = 80;
		xPoint = 0;
	}
	break;
	case rs::IndustryType::IRONOREMINE :
	{
		pointMapping = 86;
		xPoint = -4;
	}
	break;
	case rs::IndustryType::OILREFINERY :
	{
		pointMapping = 100;
		xPoint = -8;
	}
	break;
	case rs::IndustryType::OILRIG :
	{
		pointMapping = 32;
		xPoint = -32;
	}
	break;
	case rs::IndustryType::OILWELLS :
	{
		pointMapping = 32;
		xPoint = 0;
	}
	break;
	case rs::IndustryType::SAWMILL :
	{
		pointMapping = 25 + 32;
		xPoint = -2;
	}
	break;
	case rs::IndustryType::STEELMILL :
	{
		pointMapping = 64;
		xPoint = 0;
	}
	break;
	default:
		break;
	}

	float x1, y1;
	x1 = this->m_x;
	y1 = this->m_y;
	rs::twoDToIso(x1, y1, 64, 32);

	m_sprite.setTexture(*m_texture);
	m_sprite.setPosition(sf::Vector2f(x1 + xPoint, y1 - (m_texture->getSize().y) + pointMapping));


}

void Industry::setProp(const rs::IndustryType type)
{
    srand(time(NULL));

    switch (type) {

    using namespace rs;

    case IndustryType::COALMINE:
    {
        m_iResNum = 0; // No input resources

        m_oResNum = 1;
        m_oRsrc[0] = Resources::COAL;

        m_isActive = true;
        m_workSpeed = 10;

    }break;
    case IndustryType::POWERSTATION:
    {
		m_oResNum = 0;

        m_iResNum = 1;
        m_iRsrc[0] = Resources::COAL;

		m_isActive = true;
        m_workSpeed = 1;

    }break;
    case IndustryType::FACTORY:
    {
        m_iResNum = 3;
        m_oResNum = 1;

		m_oRsrc[0] = Resources::GOODS;

        m_iRsrc[0] = Resources::GRAIN;
		m_iRsrc[1] = Resources::LIVESTOCK;
		m_iRsrc[2] = Resources::STEEL;

		m_isActive = true;
        m_workSpeed = 10;

    }break;
	case IndustryType::BANK:
	{
		m_iResNum = 1;
		m_oResNum = 0;

		m_iRsrc[0] = Resources::VALUABLES;

		m_isActive = true;
		m_workSpeed = 1;
	}
	break;
	case IndustryType::IRONOREMINE:
	{
		m_iResNum = 0;
		m_oResNum = 1;

		m_oRsrc[0] = Resources::IRONORE;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::FARM:
	{
		m_iResNum = 0;
		m_oResNum = 2;

		m_oRsrc[0] = Resources::LIVESTOCK;
		m_oRsrc[1] = Resources::GRAIN;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::FOREST:
	{
		m_iResNum = 0;
		m_oResNum = 1;

		m_oRsrc[0] = Resources::WOOD;

		m_isActive = true;
		m_workSpeed = 10;

	}
	break;
	case IndustryType::OILWELLS:
	{
		m_iResNum = 0;
		m_oResNum = 1;

		m_oRsrc[0] = Resources::OIL;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::OILRIG:
	{
		m_iResNum = 2;
		m_oResNum = 3;

		m_iRsrc[0] = Resources::MAIL;
		m_iRsrc[1] = Resources::PASSANGERS;

		m_oRsrc[0] = Resources::OIL;
		m_oRsrc[1] = Resources::PASSANGERS;
		m_oRsrc[2] = Resources::MAIL;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::OILREFINERY:
	{
		m_iResNum = 1;
		m_oResNum = 1;

		m_iRsrc[0] = Resources::OIL;
		m_oRsrc[0] = Resources::GOODS;
		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::SAWMILL:
	{
		m_iResNum = 1;
		m_oResNum = 1;

		m_iRsrc[0] = Resources::WOOD;
		m_oRsrc[0] = Resources::GOODS;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
	case IndustryType::STEELMILL:
	{
		m_iResNum = 1;
		m_oResNum = 1;

		m_iRsrc[0] = Resources::IRONORE;
		m_oRsrc[0] = Resources::STEEL;

		m_isActive = true;
		m_workSpeed = 10;
	}
	break;
    default:
    {
        m_workSpeed = 0;
    }
	break;
    }
}

void Industry::setIsActive()
{
    m_isActive = true;
}

GreeneryObject::GreeneryObject(rs::Greenery structure, float x, float y)
{
	srand(time(NULL));

	m_structure = structure;
	m_spriteNum = rand() % 4; 
	m_texture = &m_structure.spriteSet.at(m_spriteNum);
	createObject(rs::ObjectType::GREENERY, m_texture, x, y);


	m_sprite.setTexture(*m_texture);
	m_timeSinceLastChange = 0;

	float n_x = x;
	float n_y = y;
	rs::twoDToIso(n_x, n_y, 64, 32);

	m_sprite.setPosition(n_x + m_structure.offsetX, n_y + m_structure.offsetY);

}

void GreeneryObject::update(const float dt)
{
	m_timeSinceLastChange += dt;
	int x = (int)rand() % 20;
	if (x == 10 & m_timeSinceLastChange > 2.5)
	{
		m_timeSinceLastChange = 0;
		m_spriteNum = (m_spriteNum + 1) % 4;
		m_sprite.setTexture(m_structure.spriteSet.at(m_spriteNum));
	}
}

void GreeneryObject::draw(sf::RenderWindow & view)
{
	view.draw(this->m_sprite);
}

void GreeneryObject::loadObject(sf::Texture * texture)
{
}

void GreeneryObject::loadObject(rs::Greenery greeneryStruct)
{
	rs::GreeneryType type = greeneryStruct.type;
	m_structure = greeneryStruct;
	m_structure.type = type;

	m_texture = &m_structure.spriteSet.at(m_spriteNum);
	m_sprite.setTexture(*m_texture);

	m_timeSinceLastChange = 0;
	float n_x = m_x;
	float n_y = m_y;
	rs::twoDToIso(n_x, n_y, 64, 32);
	m_sprite.setPosition(n_x + m_structure.offsetX, n_y + m_structure.offsetY);
}
