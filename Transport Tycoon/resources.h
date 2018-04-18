#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <deque>
#include <utility>

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


/////////////////////////////////////////////////////////////////////////////////////////
/// NAMESPACE RS
/// This header file contains all important declaration of enum, global constants and
/// common structures.
///


namespace rs {

//const int G_SPR_NUM_REQ = 8; // Required sprite amount for correct work
const float G_PROGRAM_VERSION = 1.0;
const int   G_DEFAULT_MAP_SIZE = 256;

enum class TileType {VOID, WATER, DEEPWATER,SAND, PLAIN, FOREST,STONE,
                     ROCKS, SNOW};

enum class ObjectType {INDUSTRY ,ROAD, VECHICALE};

enum class Resources {COAL, IRONORE, GRAIN, LIVESTOCK, STEEL, WOOD, OIL,
                      GOODS, PASSAGERS, MAIL, VALUABLES};

enum class RoadType {   ROAD_0_PATH1, ROAD_1_PATH1, ROAD_1_PATH2, ROAD_1_PATH3, ROAD_1_PATH4,
                        ROAD_2_PATH1, ROAD_2_PATH2, ROAD_2_PATH3, ROAD_2_PATH4, ROAD_2_PATH5, ROAD_2_PATH6,
                        ROAD_3_PATH1, ROAD_3_PATH2, ROAD_3_PATH3, ROAD_3_PATH4, ROAD_4_PATH1  };

enum class IndustryType {COALMINE, POWERSTATION, IRONOREMINE, STEELMILL,
                         FARM, FACTORY, FOREST, SAWMILL, OILWELLS,
                         OILRIG, OILREFINERY, BANK};

enum class MapSprites {DEEP_WATER, WATER, SAND, PLAIN, FOREST, STONE,
                       MOUNTAIN_STONE, SNOW};

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

};

struct PPoint   // Point struct for path finding algorithm
{
    int x,y;    //  Position
    float g;    //  Cost of the path from the start point
    float h;    //  Heuristic cost estimate
    float f;    //  Sum of H and F

    PPoint* cameFrom;
};


struct Point
{
    int x;
    int y;

    void setValues(int nx, int ny)
    {
        x = nx;
        y = ny;
        return;
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
    char m_map[4][4];


    void CreateIndustryMap(int x, int y, const char map[4][4]){
        m_sizeX = x;
        m_sizeY = y;


        for(int i = 0; i < m_sizeX; ++i)
            for(int j = 0; j < m_sizeY; ++j)
            {
                m_map[i][j] = map[i][j];
            }

    }
};

}



#endif // RESOURCES_H
