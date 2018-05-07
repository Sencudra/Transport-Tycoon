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
	std::cout << "default " << std::endl;

}


World::World(int mode, ng::ProgramEngine* engine, ProgramStateMain *state):
    m_engine(engine), m_state(state)
{
	// io initialising 

	engine->io_setupIO(this);

	// mode loading
    if(mode == 1)
    {
        loadFromFile();
    }
    else
    {
        m_player = Player();
        m_day = 0;

        int mapSize = std::stoi(engine->m_iniFile.get<std::string>("game.default_map_size"));
        if(mapSize < 128 || mapSize > 4096) mapSize = rs::G_DEFAULT_MAP_SIZE;
        m_tileMap = new Map(this, engine, mapSize);
    }

    m_oneDayTimer = 0;
    m_isPause = false;
	m_isSpeed = false;
	m_drawFlag = false;

    m_timePerDay = 2.5;
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
        if (m_timePerDay == 0.75)
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

    float a,b;
    a = x;
    b = y;
    rs::isoToTwoD(a, b,64, 32);

    int x1 , y1;

    x1 = a;
    y1 = b;
    if((x1 <= 0 || x1 >= m_tileMap->getMapSize()) || (y1 <= 0 || y1 >= m_tileMap->getMapSize())) return;

    /* checking for tile availableness */
    bool isTileAvailable;

    (m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::DEEPWATER &&
      m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::WATER &&
      m_tileMap->m_map[x1][y1]->m_tileType != rs::TileType::ROCKS &&
      m_tileMap->m_map[x1][y1]->m_tileStatObj == NULL)
      ? isTileAvailable = true : isTileAvailable = false;

    /* adding new road path */
    if(isTileAvailable)
    {
        /* check balance*/
        if(!m_player.getMoney(100)) return;

        std::cout << "Add road" << std::endl;
        Road* road = new Road(rs::ObjectType::ROAD, m_engine->m_texmng->getTextureRef(rs::RoadType::ROAD_0_PATH1), rs::RoadType::ROAD_0_PATH1);
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

    float x_iso, y_iso;
	x_iso = x;
	y_iso = y;

    rs::isoToTwoD(x_iso, y_iso, 64, 32);

    int x_2d , y_2d;
    x_2d = x_iso;
    y_2d = y_iso;

    if((x_2d <= 0 || x_2d >= m_tileMap->getMapSize()) || (y_2d <= 0 || y_2d >= m_tileMap->getMapSize())) return nullptr;



    /* checking for tile availableness */
    bool isTileAvailable;

    (m_tileMap->m_map[x_2d][y_2d]->m_tileStatObj != NULL &&
            m_tileMap->m_map[x_2d][y_2d]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
      ? isTileAvailable = true : isTileAvailable = false;

    /* adding new vechicale */
    if(isTileAvailable)
    {
        /* Check balance */
        if(!m_player.getMoney(250)) return nullptr;

        DynamicObject* car = new DynamicObject(&m_player, m_tileMap, m_engine->m_texmng->getTextureRef("auto"), float(x_2d), float(y_2d));
        m_objDynamContainer.push_back(car);
		m_tileMap->m_map[x_2d][y_2d]->m_tileDynObj.push_back(car);

        return car;
    }
    else
    {
        return nullptr;
    }
}

void World::loadFromFile()
{
    //m_tileMap
    //m_day = 0;
    //m_player = Player();


    // Create a root
    boost::property_tree::ptree root;

    // Load the json file in this ptree
    boost::property_tree::read_json("GameSave.json", root);

    //GAME
    m_day = root.get<int>("world.day");

    //PLAYER
    std::string companyName = root.get<std::string>("world.player.company_name");
    std::string playerName = root.get<std::string>("world.player.player_name");
    int balance = root.get<int>("world.player.player_balance");

    m_player = Player(balance, companyName,playerName);

    int mapSize = root.get<int>("map.map_size");

    //std::cout << "Loading... " << std::endl;
    //std::cout << "MapSize: " << mapSize << std::endl;


    //isPause

    m_tileMap = new Map(this,m_engine);
    m_tileMap->m_map = new Tile** [mapSize];
    m_tileMap->setMapSize(mapSize);


    int x = -1;
    for(boost::property_tree::ptree::value_type &row : root.get_child("map"))
    {
        m_tileMap->m_map[x] = new Tile* [mapSize];
        int y = 0;
        for(boost::property_tree::ptree::value_type &tile : row.second)
        {
            int height = tile.second.get<int>("tile_height");
            int roadType = tile.second.get<int>("road_type");

            int x1 = tile.second.get<int>("i");
            int y1 = tile.second.get<int>("j");


            rs::TileType tileType = (rs::TileType)tile.second.get<int>("tile_type");

            //rs::TileType tileType = getTileType(height);
            //std::cout<< "DATA" << x1 << " " << y1 << std::endl;
            m_tileMap->m_map[x1][y1] = new Tile(height, m_tileMap->getTileTexture(height), tileType, &m_drawFlag);


            if(roadType != -1)
            {
                rs::RoadType type  = (rs::RoadType) roadType;
                Road* road = new Road(rs::ObjectType::ROAD, m_engine->m_texmng->getTextureRef(type), type);
                m_tileMap->m_map[x1][y1]->setObject(road);
                int a,b;
                a = x1;
                b = y1;
                rs::twoDToIso(a,b,64,32);
                m_tileMap->m_map[x1][y1]->m_sprite.setPosition(a,b);
                road->m_sprite.setPosition(sf::Vector2f(a,b));


            }
            y++;
        }
        x++;
    }

    //    for (boost::property_tree::ptree::value_type &object : root.get_child("world").get_child("objects"))
    //    {
    //        DynamicObject* nObj = dynamic_cast<DynamicObject*> (obj);
    //        boost::property_tree::ptree dynObject;
    //        int object_type = (int) nObj->m_objectType;
    //        dynObject.put("x",nObj->m_x);
    //        dynObject.put("y",nObj->m_y);
    //        dynObject.put("type",object_type);
    //        dynObject.put("cargoLoaded", nObj->m_cargoLoaded);
    //        dynObject.put("capacity", nObj->m_capacity);

    //        boost::property_tree::ptree moveTask;
    //        for(auto& i : nObj->m_moveTask)
    //        {
    //            boost::property_tree::ptree point;
    //            point.put("x",i.x);
    //            point.put("y",i.y);
    //            moveTask.add_child("point", point);
    //        }
    //        dynObject.add_child("moveTask", moveTask);
    //        objects.add_child("dynObject", dynObject);
    //    }
}

void World::saveToFile()
{
    boost::property_tree::ptree root;

    /* World DATA */
    boost::property_tree::ptree world;
    world.put("day", this->m_day);

    /* Player DATA*/
    boost::property_tree::ptree player;

    player.put("player_name", m_player.getPlayerName());
    player.put("company_name", m_player.getCompanyName());
    player.put("player_balance", m_player.getBalance());

    world.add_child("player", player);

    /* Objects DATA */
    boost::property_tree::ptree objects;


    for (auto &obj : m_objDynamContainer)
    {
        DynamicObject* nObj = dynamic_cast<DynamicObject*> (obj);
        boost::property_tree::ptree dynObject;
        int object_type = (int) nObj->m_objectType;
        dynObject.put("x",nObj->m_x);
        dynObject.put("y",nObj->m_y);
        dynObject.put("type",object_type);
        dynObject.put("cargoLoaded", nObj->m_cargoLoaded);
        dynObject.put("capacity", nObj->m_capacity);

        boost::property_tree::ptree moveTask;
        for(auto& i : nObj->m_moveTask)
        {
            boost::property_tree::ptree point;
            point.put("x",i.x);
            point.put("y",i.y);
            moveTask.add_child("point", point);
        }
        dynObject.add_child("moveTask", moveTask);
        objects.add_child("dynObject", dynObject);
    }

    // Add the new node to the root
    world.add_child("objects", objects);


    /* Map DATA*/

    boost::property_tree::ptree map;

    int size = m_tileMap->getMapSize();
    map.put("map_size",size);

    for(int i = 0; i < size; ++i)
    {
        boost::property_tree::ptree row;
        for(int j = 0; j < size; ++j)
        {
            boost::property_tree::ptree tile;

            int height =  m_tileMap->m_map[i][j]->m_tileHeight;
            int type = (int) m_tileMap->m_map[i][j]->m_tileType;
            int roadType = -1;
            int tileType = (int) m_tileMap->m_map[i][j]->m_tileType;

            if(m_tileMap->m_map[i][j]->m_tileStatObj != NULL &&
               m_tileMap->m_map[i][j]->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
            {
                Road* road = dynamic_cast<Road*> (m_tileMap->m_map[i][j]->m_tileStatObj);
                roadType = (int) road->getType();
            }

            tile.put("j",j);
            tile.put("i",i);
            tile.put("tile_height", height);
            tile.put("tile_type", type);
            tile.put("road_type", roadType);
            tile.put("tile_type", tileType);
            //tile.put(*m_tileMap->m_map[i][j]);

            row.push_back(std::make_pair("", tile));
        }
        /* Add the row to our matrix */
        map.push_back(std::make_pair("row", row));
    }
    root.add_child("world", world);
    root.add_child("map", map);


    boost::property_tree::write_json("GameSave.json", root);
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
				//std::cout << "X: " << x << "Y: " << y << std::endl;
			}
		
		}
	}	
	return;
}




