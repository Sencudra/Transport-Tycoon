#pragma once

#ifndef RESOURCES_H
#define RESOURCES_H

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <utility>

#include <glut.h> // OpenG

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

//#include <boost/assert.hpp> 
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/base_object.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/vector.hpp>

#include "imgui-sfml.h"


/////////////////////////////////////////////////////////////////////////////////////////
/// NAMESPACE RS
/// This header file contains all important declaration of enum, global constants and
/// common structures.
///


namespace rs {

	//const int G_SPR_NUM_REQ = 8; // Required sprite amount for correct work
	const float G_PROGRAM_VERSION = 1.0;
	const int   G_DEFAULT_MAP_SIZE = 256;
	const char  G_PROGRAM_NAME[20] = "Transport Paradise";



	/////////////////////////////////////////////////////////////////////////////////////////
	// TileType and it's serializations method
	//

	enum class TileType {
		VOID, WATER, DEEPWATER, SAND, PLAIN, FOREST, STONE,
		ROCKS, SNOW
	};

	enum class GreeneryType {
		TREE_1, TREE_2, TREE_3
	};

	struct Greenery
	{
		rs::GreeneryType type;
		int offsetX;
		int offsetY;
		std::vector<sf::Texture> spriteSet;
	};

enum class ObjectType {INDUSTRY ,ROAD, VEHICLE, GREENERY};

enum class Resources {COAL, IRONORE, GRAIN, LIVESTOCK, STEEL, WOOD, OIL,
                      GOODS, PASSANGERS, MAIL, VALUABLES};

enum class RoadType {   ROAD_0_PATH1, ROAD_1_PATH1, ROAD_1_PATH2, ROAD_1_PATH3, ROAD_1_PATH4,
                        ROAD_2_PATH1, ROAD_2_PATH2, ROAD_2_PATH3, ROAD_2_PATH4, ROAD_2_PATH5, ROAD_2_PATH6,
                        ROAD_3_PATH1, ROAD_3_PATH2, ROAD_3_PATH3, ROAD_3_PATH4, ROAD_4_PATH1  };

enum class VehicleType { BUS, MAIL_TRUCK, OIL_TANKER, GOODS_TRUCK, GRAIN_TRUCK, LIVESTOCK_VAN,
						WOOD_TRUCK, COAL_TRUCK, IRON_ORE_TRUCK, STEEL_TRUCK };

enum class IndustryType {COALMINE, POWERSTATION, IRONOREMINE, STEELMILL,
                         FARM, FACTORY, FOREST, SAWMILL, OILWELLS,
                         OILRIG, OILREFINERY, BANK};

enum class MapSprites {DEEP_WATER, WATER, SAND, PLAIN, FOREST, STONE,
                       MOUNTAIN_STONE, SNOW};

enum class ActionState { NONE, PANNING };

enum class EditState { NONE, ROADING, CARSETUP, ROUTING, DELETING };

enum class Color { WHITE, RED, GREEN, NONE};


template <typename T>
void twoDToIso(T &x, T &y,size_t tileWSize, size_t tileHSize)
{
    T a = x;
    T b = y;
    x = (a - b)*tileWSize/2;
    y = (a + b)*tileHSize/2;
    return;
}

template <typename T>
void isoToTwoD(T &x, T& y, size_t tileWSize, size_t tileHSize)
{
    T a = x - tileWSize/2;
    T b = y;
    x = (a / (tileWSize*0.5) + b / (tileHSize*0.5)) /2;
    y = (b / (tileHSize*0.5) - a / (tileWSize*0.5)) /2;
    return;
}

struct Cargo
{
  int size;
  Resources type;

  template<class Archive>
  void serialize(Archive & ar, const unsigned int version)
  {
	  ar & size;
	  ar & type;
  }
};

struct Point
{
    int x;
    int y;

	//Point(int x_new,int y_new)
	//{
	//	x = x_new;
	//	y = y_new;
	//}

    void setValues(int nx, int ny)
    {
        x = nx;
        y = ny;
        return;
    }

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & x;
		ar & y;
	}

};

struct Rectangle
{
	Point topRight;
	Point topLeft;
	Point bottomLeft;
	Point bottomRight;
};

struct ScreenRectangle
{
    Point topRight;
    Point topLeft;
    Point bottomLeft;
    Point bottomRight;
};

struct IndustryMap
{
    int m_sizeX;
    int m_sizeY;
    char m_map[4][5];


    void CreateIndustryMap(int x, int y, const char map[4][5]){
        m_sizeX = x;
        m_sizeY = y;

        for(int i = 0; i < m_sizeX; ++i)
            for(int j = 0; j < m_sizeY; ++j)
            {
                m_map[i][j] = map[i][j];
            }

    }
};


namespace boost {
	namespace serialization {

		template<class Archive, class T>
		void serialize(Archive & ar, T & c, const unsigned int version)
		{
			ar & int(c);
		}
	} // namespace serialization
} // namespace boost



}

class Player;

namespace vhs {

	enum class enumVehicle { BALOGH, UHL, DW };
	enum class Directions { TOP_LEFT, BOTTOM_LEFT, BOTTOM_RIGHT, TOP_RIGHT};

	struct Vehicle
	{
		int id;						// ATTENTION: null after loading
		vhs::enumVehicle name;		// name
		Player* owner;				// ATTENTION: null after loading
		float price;				// in dollars
		int speed;					// km/h
		int runCost;				// Per year
		int dateDesigned;			// Year
		int lifespan;				// In years
		int capacity;				// In tonnes
		rs::Resources resource;
		std::map<vhs::Directions, sf::Texture>* sprites;
	};
}



#endif // RESOURCES_H
