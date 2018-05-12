#include "resources.h"
#include "world.h"
#include "programstatemain.h"
#include "tile.h"

#include <math.h>
#include "object.h"
#include "player.h"


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

World::World()
{
	m_drawFlag = false;
	m_timePerDay = 2.5;
	m_oneDayTimer = 0;

}


World::World(ng::ProgramEngine* engine, ProgramStateMain *state):
    m_engine(engine), m_state(state)
{
	// io initialising 

	engine->io_setupIO(this);

    m_player = Player();
    m_day = 0;

    int mapSize = std::stoi(engine->m_iniFile.get<std::string>("game.default_map_size"));
    if(mapSize < 128 || mapSize > 4096) mapSize = rs::G_DEFAULT_MAP_SIZE;
    m_tileMap = new Map(this, engine, mapSize);


    m_oneDayTimer = 0;
    m_isPause = false;
	m_isSpeed = false;
	m_drawFlag = false;

    m_timePerDay = 2.5;
}

void World::WorldLoadSetup(ng::ProgramEngine* engine, ProgramStateMain* state)
{
	engine->io_setupIO(this); // Input output module

	this->m_engine = engine;
	this->m_state = state;

	 m_drawFlag = false; // tile rendering flag

	m_timePerDay = 2.5;
	m_oneDayTimer = 0;

	for (auto i : m_objStaticContainer)
	{
		if (i->m_objectType == rs::ObjectType::ROAD)
		{
			Road* road = dynamic_cast<Road*>(i);
			road->loadObject(m_engine->m_texmng->getTextureRef(road->getType()));
			m_tileMap->m_map[road->m_x][road->m_y]->setObject(road);
			m_tileMap->m_map[road->m_x][road->m_y]->isMainStatic = true;
		}
		if (i->m_objectType == rs::ObjectType::INDUSTRY)
		{
			Industry* ind = dynamic_cast<Industry*>(i);
		
			ind->loadObject(m_engine->m_texmng->getTextureRef(ind->getType()));
			m_tileMap->placeIndustry(ind);

		}
		if (i->m_objectType == rs::ObjectType::GREENERY)
		{
			GreeneryObject* green = dynamic_cast<GreeneryObject*>(i);
			rs::Greenery greeneryStruct = m_engine->m_texmng->getGreeneryStruct(green->getGreeneryType());

			m_tileMap->m_map[green->m_x][green->m_y]->setObject(green);
			m_tileMap->m_map[green->m_x][green->m_y]->isMainStatic = true;
			green->loadObject(greeneryStruct);

		}
	}
	for (auto i : m_objDynamContainer)
	{
		if (i->m_objectType == rs::ObjectType::VEHICLE)
		{
			Vehicle* dy = dynamic_cast<Vehicle*> (i);
			vhs::Vehicle vehStruct = m_engine->m_texmng->getVehicleStruct(dy->getVehicleType());
			vehStruct.owner = &m_player;
			dy->loadObject(vehStruct, m_tileMap);
		}

	}



}


World::~World()
{
    for(Object* i : m_objStaticContainer)
    {
        delete i;
    }
}

void World::update(float dt)
{
    if(dt > 0.5)return;
    if(!m_isPause){
        float time_dt = dt*2;
        if (m_timePerDay == 1.25)
            time_dt = dt*4;
        
		for(auto i : m_objStaticContainer)
            i->update(time_dt);

        for(auto i : m_objDynamContainer)
        {
            i->update(time_dt);
        }

        this->m_oneDayTimer += dt;
        if(this->m_oneDayTimer < m_timePerDay) return;
        else
        {
            m_day++;
            std::cout << m_day << std::endl;
            m_oneDayTimer = 0;
        }
    }




}

void World::draw(ScreenView& gameView)
{
    
	drawMap(gameView);

}

void World::addRoad(float x, float y)
{

    float a,b; a = x; b = y;
    rs::isoToTwoD(a, b,64, 32);

    int x1 , y1; x1 = a; y1 = b;
    if((x1 <= 0 || x1 >= m_tileMap->getMapSize()) || (y1 <= 0 || y1 >= m_tileMap->getMapSize())) return;

    /* checking for tile availableness */
    bool isTileAvailable;

    (m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::DEEPWATER &&
      m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::WATER &&
      m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::ROCKS &&
      (m_tileMap->m_map[x1][y1]->m_tileStatObj == NULL ||
		m_tileMap->m_map[x1][y1]->m_tileStatObj->m_objectType == rs::ObjectType::GREENERY))
      ? isTileAvailable = true : isTileAvailable = false;

    /* adding new road path */
    if(isTileAvailable)
    {
        /* check balance*/
        if(!m_player.getMoney(100)) return;

        std::cout << "Add road" << std::endl;
        Road* road = new Road(rs::ObjectType::ROAD, m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_0_PATH1),
			rs::RoadType::ROAD_0_PATH1, x1, y1);
        m_tileMap->m_map[x1][y1]->setObject(road);
		m_tileMap->m_map[x1][y1]->isMainStatic = true;

		this->m_objStaticContainer.push_back(road);


		// Update road image
        updateRoadDirection(x1,y1);

        int mapSize = m_tileMap->getMapSize();

        if(x1-1 > 0 && m_tileMap->m_map[x1-1][y1]->m_tileStatObj != NULL && m_tileMap->m_map[x1-1][y1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
            updateRoadDirection(x1-1, y1);

        if(y1-1 > 0 && m_tileMap->m_map[x1][y1-1]->m_tileStatObj != NULL && m_tileMap->m_map[x1][y1-1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
            updateRoadDirection(x1, y1-1);

        if(x1+1 < mapSize && m_tileMap->m_map[x1+1][y1]->m_tileStatObj != NULL && m_tileMap->m_map[x1+1][y1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
            updateRoadDirection(x1+1,y1);

        if(y1+1 < mapSize && m_tileMap->m_map[x1][y1+1]->m_tileStatObj != NULL && m_tileMap->m_map[x1][y1+1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
           updateRoadDirection(x1, y1+1);
    }
}

void World::updateRoadDirection(int a, int b)
{
    bool TL, TR, BL, BR;
    TL = TR = BL = BR = false;
    int mapSize = m_tileMap->getMapSize();


    if (a > 0 && a < mapSize && b > 0 && b < mapSize)

    if(a > 0 && m_tileMap->m_map[a-1][b]->m_tileStatObj != NULL && m_tileMap->m_map[a-1][b]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
        TL = true;
    if(b > 0 && m_tileMap->m_map[a][b-1]->m_tileStatObj != NULL && m_tileMap->m_map[a][b-1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
        TR = true;
    if(a < mapSize && m_tileMap->m_map[a+1][b]->m_tileStatObj != NULL && m_tileMap->m_map[a+1][b]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
        BR = true;
    if(b < mapSize && m_tileMap->m_map[a][b+1]->m_tileStatObj != NULL && m_tileMap->m_map[a][b+1]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
        BL = true;

    Road* road = dynamic_cast<Road*> (m_tileMap->m_map[a][b]->m_tileStatObj);
    sf::Texture* newTexture;

    /* No Roads */
    if(BR == false && TR == false &&  BL == false && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_0_PATH1);
        road->setNewType(newTexture, rs::RoadType::ROAD_0_PATH1);
    }
    /* 4 Roads */
    else if(BR == true && TR == true &&  BL == true && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_4_PATH1);
        road->setNewType(newTexture, rs::RoadType::ROAD_4_PATH1);
    }


    /* 1 Path Roads */
    else if(BR == false && TR == false &&  BL == false && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_1_PATH1);
        road->setNewType(newTexture, rs::RoadType::ROAD_1_PATH1);
    }
    else if(BR == true && TR == false &&  BL == false && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_1_PATH2);
        road->setNewType(newTexture, rs::RoadType::ROAD_1_PATH2);
    }
    else if(BR == false && TR == true &&  BL == false && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_1_PATH3);
        road->setNewType(newTexture, rs::RoadType::ROAD_1_PATH3);
    }
    else if(BR == false && TR == false &&  BL == true && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_1_PATH4);
        road->setNewType(newTexture, rs::RoadType::ROAD_1_PATH4);
    }


    /* 2 Path Roads */
    else if(BR == true && TR == false &&  BL == false && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH1);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH1);
    }
    else if(BR == false && TR == true &&  BL == true && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH2);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH2);
    }
    else if(BR == false && TR == true &&  BL == false && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH3);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH3);
    }
    else if(BR == true && TR == false &&  BL == true && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH4);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH4);
    }
    else if(BR == false && TR == false &&  BL == true && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH5);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH5);
    }
    else if(BR == true && TR == true &&  BL == false && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_2_PATH6);
        road->setNewType(newTexture, rs::RoadType::ROAD_2_PATH6);
    }


    /* 3 Path Roads */
    else if(BR == false && TR == true &&  BL == true && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_3_PATH1);
        road->setNewType(newTexture, rs::RoadType::ROAD_3_PATH1);
    }
    else if(BR == true && TR == false &&  BL == true && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_3_PATH2);
        road->setNewType(newTexture, rs::RoadType::ROAD_3_PATH2);
    }
    else if(BR == true && TR == true &&  BL == true && TL == false)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_3_PATH3);
        road->setNewType(newTexture, rs::RoadType::ROAD_3_PATH3);
    }
    else if(BR == true && TR == true &&  BL == false && TL == true)
    {
        newTexture = m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_3_PATH4);
        road->setNewType(newTexture, rs::RoadType::ROAD_3_PATH4);
    }
    else
    {
        std::cout << "Update direction error" << std::endl;
    }
}

Object* World::addVehicle(float x, float y)
{
	
	// cheking for right edges
	float x_iso, y_iso; x_iso = x; y_iso = y;
    rs::isoToTwoD(x_iso, y_iso, 64, 32);

    int x_2d , y_2d; x_2d = x_iso; y_2d = y_iso;
	if ((x_2d <= 0 || x_2d >= m_tileMap->getMapSize()) ||
		(y_2d <= 0 || y_2d >= m_tileMap->getMapSize()))
	{
		return nullptr;
	}

    /* adding new vechicale */
    if(isTileIsRoad(x_2d, y_2d))
    {
        /* Check balance */
        if(!m_player.getMoney(250)) return nullptr;

		// Temporary
		vhs::enumVehicle eV = vhs::enumVehicle::BALOGH;
		vhs::Vehicle veh = this->m_engine->m_texmng->getVehicleStruct(eV);
		veh.owner = &m_player;


        Vehicle* car = new Vehicle(veh, m_tileMap, float(x_2d), float(y_2d));
        m_objDynamContainer.push_back(car);
		m_tileMap->m_map[x_2d][y_2d]->m_tileDynObj.push_back(car);

        return car;
    }
    else
    {
        return nullptr;
    }
}

bool World::isTileIsRoad(int x, int y)
{
	if (m_tileMap->m_map[x][y]->m_tileStatObj != nullptr &&
		m_tileMap->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void World::deleteVec(Object* obj)
{
    int position = 0;
    for(auto i : m_objDynamContainer)
    {
        if(i == obj)
        {
            m_objDynamContainer.erase(m_objDynamContainer.begin()+position);
			return;
        }
        position++;
    }

	int x1, y1;

	x1 = obj->m_x;
	y1 = obj->m_y;
	m_tileMap->m_map[x1][y1]->deleteObject();

	for (auto i : m_tileMap->m_map[x1][y1]->m_tileDynObj)
	{
		if (i == obj)
		{
			m_tileMap->m_map[x1][y1]->m_tileDynObj.erase(m_tileMap->m_map[x1][y1]->m_tileDynObj.begin() + position);
			delete i;
			return;
		}
		position++;
	}
}


void World::deleteObject(sf::Vector2f pos)
{
    float x,y;
    x = pos.x;
    y = pos.y;
    rs::isoToTwoD(x,y,64,32);
    int x1,y1;

    x1 = x;
    y1 = y;

	int j = 0;
	for (auto i : m_objStaticContainer)
	{
		if (i->m_objectType == rs::ObjectType::ROAD && x1 == i->m_x && y1 == i->m_y)
		{
			m_objStaticContainer.erase(m_objStaticContainer.begin() + j);
			break;
		}
		++j;
	}

	m_tileMap->m_map[x1][y1]->deleteObject();
	

    for(int x = x1 - 1; x < x1 + 1; ++x )
        for(int y = y1 - 1; y < y1 + 1; ++y)
        {
            if(x != x1 && y != y1)
            {
                if(m_tileMap->m_map[x][y]->m_tileStatObj!=NULL && m_tileMap->m_map[x][y]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
                    this->updateRoadDirection(x,y);
            }
        }

}


Object* World::selectObject(sf::Vector2f pos)
{

	// Object Highlight, Color Green
    for(auto i : m_objDynamContainer)
    {
        if(i->m_sprite.getGlobalBounds().contains(pos.x, pos.y))
        {
            if(!i->m_isSelected)
            {
                i->m_sprite.setColor(Color::Green);
                i->m_isSelected = true;
                return i;
            }
            else
            {
                i->m_sprite.setColor(Color::White);
                i->m_isSelected = false;
                return nullptr;
            }
        }
    }
    for(auto i : m_objStaticContainer)
    {
        if(i->m_sprite.getGlobalBounds().contains(pos.x, pos.y))
        {
            if(!i->m_isSelected)
            {
                i->m_sprite.setColor(Color::Green);
                i->m_isSelected = true;
                return i;
            }
            else
            {
                i->m_sprite.setColor(Color::White);
                i->m_isSelected = false;
                return nullptr;
            }
        }
    }
    return nullptr;
}

int min(int a, int b, int c)
{
	return std::min(std::min(a, b), c);
}


void World::switchPause()
{
	m_isPause == true ? m_isPause = false : m_isPause = true;
	std::cout << "World::switchPause: pause mode is " << m_isPause << std::endl;
}

void World::x2Speed()
{
	if (m_isSpeed == true) { m_timePerDay *= 2; m_isSpeed = false; }
	else { m_timePerDay /= 2; m_isSpeed = true; }
	std::cout << "World::x2Speed: speed set is " << m_timePerDay << std::endl;
}

void World::drawMap(ScreenView& gameView)
{
    using namespace rs;

	int mapSize = m_tileMap->getMapSize();
	// Object culling according to screen rectangle
    ScreenRectangle idxRec = gameView.getViewRect();
    isoToTwoD(idxRec.topLeft.x, idxRec.topLeft.y, 64, 32);
    isoToTwoD(idxRec.topRight.x, idxRec.topRight.y, 64, 32);
    isoToTwoD(idxRec.bottomLeft.x, idxRec.bottomLeft.y, 64, 32);
    isoToTwoD(idxRec.bottomRight.x,idxRec.bottomRight.y, 64, 32);


	//Buffer 

    idxRec.bottomLeft.y += 2;


	int dRows = std::abs(idxRec.topRight.y - idxRec.bottomLeft.y);
	int dColumns = std::abs(idxRec.topLeft.x - idxRec.bottomRight.x);

    if(idxRec.topLeft.x > mapSize)			idxRec.topLeft.x = mapSize;
    if(idxRec.topLeft.x < 0)                idxRec.topLeft.x = 0;

    if(idxRec.topRight.y > mapSize)			idxRec.topRight.y = mapSize;
    if(idxRec.topRight.y < 0)               idxRec.topRight.y = 0;

    if(idxRec.bottomLeft.y > mapSize)		idxRec.bottomLeft.y = mapSize;
    if(idxRec.bottomLeft.y < 0)             idxRec.bottomLeft.y = 0;

    if(idxRec.bottomRight.x > mapSize)		idxRec.bottomRight.x = mapSize;
    if(idxRec.bottomRight.x < 0)            idxRec.bottomRight.x = 0;


	// Tiles draw for one time helper
	m_drawFlag = !m_drawFlag;

	for (int line = 1; line <= (dRows + dColumns)-1; line++)
	{
		/* get column index of the first element in this line of output.
		the index is 0 for first "row" lines and "line - row" for remaining
		lines  */
		int start_col = std::max(0, line - dRows);

		/* get count of elements in this line. the count of elements is
		equal to minimum of line number, "col-start_col" and "row" */
		int count = min(line, (dRows - start_col), dRows);

		/* print elements of this line */
		for (int j = 0; j < count; j++)
		{

			int x, y;
			x = idxRec.topLeft.x + start_col + j;
			y = idxRec.topRight.y + std::min(dRows, line) - 1 - j;

			if (x >= mapSize || y >= mapSize)
				continue;

			float n_x = x;
			float n_y = y;

			twoDToIso(n_x, n_y, 64, 32);

			if ((n_x >= gameView.getViewRect().topLeft.x - 64 &&
				n_x <= gameView.getViewRect().bottomRight.x + 64) &&
				(n_y >= gameView.getViewRect().topLeft.y - 64 &&
					n_y <= gameView.getViewRect().bottomRight.y + 64))
			{
				m_tileMap->m_map[x][y]->draw(n_x, n_y, (m_engine->m_window));
			}
		
		}
	}	
	return;
}




