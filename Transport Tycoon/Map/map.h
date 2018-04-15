#ifndef MAP_H
#define MAP_H

#include "resources.h"


namespace ng { class ProgramEngine;}
class Object;
class Tile;
class World;


class Map
{
public:

    /* Default constructor. Do nothing. */
    Map();

    Map(World* world, ng::ProgramEngine *engine);

    Map(World* world, ng::ProgramEngine *engine, int size);

    Map(const Map &obj);

    ~Map();

    /* Returns 4 map corners */
    rs::Rectangle getMapEdges();

    int getMapSize(){return m_mapSize;}
    void setMapSize(int size){m_mapSize = size;}

    sf::Texture &getTileTexture(const int cellHeight) const;
    rs::TileType getTileType(const int cellHeight) const;

private:

    void loadIndustryMaps();

    /* Methods that maintain tilemap */

    int generateMap();
    int initialiseMap();
    int generateObjects();

    bool isValid(int x, int y, Tile*** map, rs::IndustryType type);


    /* Methods that maintain landscape generation */

    int generateHeightMap(int size);
    int initialiseHeightMap();          // Array setup
    int processHeightMap();             // DS algorithm

    int getMapHeight(int x, int y){return (m_heightMap[x][y] + m_heightMap[x+1][y] + m_heightMap[x][y+1] +m_heightMap[x+1][y+1])/4;}

    /* Diamond Square Alghorithm for random map generating */
    int squareStep(int sideLength,int halfSide);
    void diamondStep(int sideLength, int halfSide);


public:

    Tile ***m_map;

private:

    int m_mapSize;

    int m_heightMapSize;
    float **m_heightMap;

    std::map<rs::IndustryType, rs::IndustryMap> m_industryMaps; // Маски педприятий
    //std::map<rs::TileType, int> m_tileDistrib;                // Распределение биомов

    ng::ProgramEngine* m_engine;
    World* m_world;

    // key value for map generating
    double m_range;

};

#endif // MAP_H
