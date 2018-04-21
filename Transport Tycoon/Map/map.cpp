#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <math.h>
#include <climits>
#include <cfloat>

#include "map.h"

#include "tile.h"
#include "programengine.h"
#include "world.h"
#include "object.h"


float deepWaterThreshold = 0.4f;
float shallowWaterThreshold = 0.45f;        //water
float desertThreshold = 0.47f;              //sand
float plainsThreshold = 0.60f;              //plaint
float grasslandThreshold = 0.65f;            //forest
float forestThreshold = 0.75f;              //stone
float hillsThreshold = 0.80f;               //ston2
float mountainsThreshold = 0.88f;           // snow





float dRand(double dMax)
{
    double d = (double)rand()/RAND_MAX;
    double result = (d-0.5)*dMax;
    return result;
}


Map::Map()
{

}

Map::Map(World *world, ng::ProgramEngine *engine):
    m_world(world),m_engine(engine)
{

}


Map::Map(World* world, ng::ProgramEngine* engine, int size):
    m_world(world),m_engine(engine)
{
    m_heightMapSize = size;
    generateHeightMap(size);
    generateMap();
}

Map::~Map()
{
    std::cout << "MAP::destructor..." << std::endl;


    for(int i = 0; i < m_heightMapSize; ++i)
    {
        delete m_heightMap[i];
    }
    delete m_heightMap;

    for(int i = 0; i < m_mapSize; ++i)
    {
        delete m_map[i];
    }
    delete m_map;
}

/////////////////////////////////////////////////////////////////////////////////////////
/// Map methods
///

int Map::generateMap()
{
    initialiseMap();
    //showTileDistribution();
    generateObjects();

    // Add stuff /...
    return 0;
}


int Map::initialiseMap()
{
    int size = m_mapSize;
    m_map = new Tile** [size];
    for(int i = 0; i < size; ++i)
    {
        m_map[i] = new Tile* [size];
        for(int j = 0; j < size; ++j)
        {
            int height = this->getMapHeight(i,j);

            rs::TileType tileType = getTileType(height);

            m_map[i][j] = new Tile(height, getTileTexture(height),tileType, m_world->getDrawnFlag());
        }
    }
    return 0;
}




bool Map::isValid(int x, int y, Tile*** map, rs::IndustryType type)
{
    int rows = m_industryMaps[type].m_sizeX;
    int columns = m_industryMaps[type].m_sizeY;

    for(int i = y - rows, i2 = 0; i <= y, i2 < rows; ++i,++i2)
    {
        for(int j = x, j2 = 0; j <= x + columns, j2 < columns; ++j,++j2)
        {
            if(m_industryMaps[type].m_map[i2][j2] == 'x')
            {
                if(i > 0 && i < m_mapSize && j > 0 && j < m_mapSize)
                {
                    if(map[j][i]->m_tileType == rs::TileType::PLAIN || map[j][i]->m_tileType == rs::TileType::FOREST ||
                            map[j][i]->m_tileType == rs::TileType::STONE)
                    {
                        if(map[j][i]->m_tileStatObj != NULL)
                            return false;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }
    return true;
}

int Map::generateObjects()
{

    rs::IndustryType type;

    loadIndustryMaps();

    for(int i = 0; i <= 2; ++i)
    {
        int qvote = 4;
        if( i == 1)
            type = rs::IndustryType::COALMINE;
        else
            type = rs::IndustryType::POWERSTATION;

        while(qvote > 0)
        {
            bool isCreated = false;

            while(!isCreated)
            {
                int x = (int)rand()%(this->getMapSize()-1)+1;
                int y = (int)rand()%(this->getMapSize()-1)+1;


                if (m_map[x][y]->m_tileType == rs::TileType::PLAIN || m_map[x][y]->m_tileType == rs::TileType::FOREST ||
                        m_map[x][y]->m_tileType == rs::TileType::STONE)
                {
                    isCreated = isValid(x,y,m_map,type);

                    if(isCreated)
                    {
                        float a, b; 
						a = x;  
						b = y;

                        rs::twoDToIso(a,b,64,32);

                        sf::Texture* newTexture = m_engine->m_texmng->getTextureRef(type);

                        Object* newObject = new Industries(rs::ObjectType::INDUSTRY , newTexture, type, a, b);

                        //m_world->addObject(newObject);



                        int rows = m_industryMaps[type].m_sizeX;
                        int columns = m_industryMaps[type].m_sizeY;

                        for(int i = y - rows + 1, i2 = 0; i <= y, i2 < rows; ++i,++i2)
                        {
                            for(int j = x, j2 = 0; j <= x + columns, j2 < columns; ++j,++j2)
                            {

								// Look in Map::loadIndustryMaps() for more info
                                if(m_industryMaps[type].m_map[i2][j2] == 'x')
                                {        
                                    m_map[j][i]->m_tileStatObj = newObject;
                                }
								if (m_industryMaps[type].m_map[i2][j2] == '1')
								{
									m_map[j][i]->m_sprite.setTexture(*m_engine->m_texmng->getTextureRef("bg_factory"));
									m_map[j][i]->m_tileStatObj = newObject;
									m_map[j][i]->isMainStatic = true;
								}

                            }
                        }
                        m_map[x][y]->m_sprite.setTexture(*m_engine->m_texmng->getTextureRef("bg_red"));

                    }
                }
            }
            qvote--;

        }

    }
    return 0;
}

rs::Rectangle Map::getMapEdges()
{
    rs::Rectangle rect;
    rect.bottomLeft.setValues(-m_mapSize/2*64,m_mapSize/2*32);
    rect.topLeft.setValues(0,0);
    rect.topRight.setValues(m_mapSize/2*64,m_mapSize/2*32);
    rect.bottomRight.setValues(0,m_mapSize*32);
    return rect;
}


void Map::loadIndustryMaps()
{

    const char temp_matrix1[4][4] = {{'0','0','0','0'},
                                    {'0','x','x','x'},
                                    {'x','x','x','1'},
                                    {'x','x','x','0'}};


    this->m_industryMaps[rs::IndustryType::COALMINE].CreateIndustryMap(4,4,temp_matrix1);

    const char temp_matrix2[4][4] = {{'x','x','0','0'},
                                    {'x','x','0','0'},
                                    {'x','x','0','0'},
                                    {'x','1','0','0'}};

    this->m_industryMaps[rs::IndustryType::POWERSTATION].CreateIndustryMap(4,2,temp_matrix2);


}

/////////////////////////////////////////////////////////////////////////////////////////
/// Height map methods
///

int Map::initialiseHeightMap()
{
    int size = m_heightMapSize;
    m_heightMap = new float* [size];
    for(int i = 0; i < size; ++i)
    {
        m_heightMap[i] = new float[size];
        for(int j = 0; j < size; ++j)
            m_heightMap[i][j] = 0;
    }
    return 0;
}


int Map::generateHeightMap(int size)
{
    m_mapSize = size;
    m_heightMapSize = size + 1;

    initialiseHeightMap();
    processHeightMap();

    //Map landscape normalising
    float max = FLT_MIN;
    float min = FLT_MAX;
    for (int i = 0; i < m_heightMapSize; ++i) {
        for (int j = 0; j < m_heightMapSize; ++j) {
            if (m_heightMap[i][j] > max) max = m_heightMap[i][j];
            if (m_heightMap[i][j] < min) min = m_heightMap[i][j];
        }
    }

    for(int row = 0; row < m_heightMapSize; ++row){
        for(int col = 0; col < m_heightMapSize; ++col)
        {
            // Normalisong
            m_heightMap[row][col] = (float)(m_heightMap[row][col]-min)/(max-min);

            if (m_heightMap[row][col] < deepWaterThreshold) m_heightMap[row][col] = 0;
            else if (m_heightMap[row][col] < shallowWaterThreshold) m_heightMap[row][col] = 1;
            else if (m_heightMap[row][col] < desertThreshold) m_heightMap[row][col] = 2;
            else if (m_heightMap[row][col] < plainsThreshold) m_heightMap[row][col] = 3;
            else if (m_heightMap[row][col] < forestThreshold) m_heightMap[row][col] = 4;
            else if (m_heightMap[row][col] < hillsThreshold) m_heightMap[row][col] = 5;
            else if (m_heightMap[row][col] < mountainsThreshold) m_heightMap[row][col] = 6;
            else m_heightMap[row][col] = 7;
        }
    }
    return 0;
}


int Map::processHeightMap()
{
    std::cout << "MAP::ProgramEngine..." << std::endl;

    // Initializing srand for random values :
    srand(time(NULL));

    // Defining the corners values and range value:
    m_range = 1;
    int F = 1.2 ; // smothness

    m_heightMap[0][0] = dRand(m_range);
    m_heightMap[0][m_heightMapSize-1] = dRand(m_range);
    m_heightMap[m_heightMapSize-1][0] = dRand(m_range);
    m_heightMap[m_heightMapSize-1][m_heightMapSize-1] = dRand(m_range);

    //Processing Diamond Square Alg.
    for(int sideLength = m_heightMapSize-1; sideLength >= 2; sideLength /= 2, m_range *= pow(2,-F))
    {
        int halfSide = sideLength/2;

        squareStep(sideLength, halfSide);
        diamondStep(sideLength, halfSide);
    }

    return 0;
}



int Map::squareStep(int sideLength, int halfSide)
{
    for(int x = 0; x < m_heightMapSize-1; x += sideLength)
        {
            for(int y = 0; y < m_heightMapSize-1; y += sideLength)
            {
                double avg = m_heightMap[x][y] + m_heightMap[x+sideLength][y] + m_heightMap[x][y+sideLength] + m_heightMap[x+sideLength][y+sideLength];
                avg /= 4.0;
                m_heightMap[x+halfSide][y+halfSide] = avg + dRand(m_range);
            }
        }
    return 0;
}


void Map::diamondStep(int sideLength, int halfSide)
{
    for(int x = 0; x < m_heightMapSize-1; x += halfSide)
    {
        for(int y = (x+halfSide)%sideLength; y < m_heightMapSize -1; y +=sideLength)
        {
            double avg = m_heightMap[(x-halfSide+m_heightMapSize-1)%(m_heightMapSize-1)][y] +
                m_heightMap[(x+halfSide)%(m_heightMapSize-1)][y] +
                m_heightMap[x][(y+halfSide)%(m_heightMapSize-1)] +
                m_heightMap[x][(y-halfSide+m_heightMapSize-1)%(m_heightMapSize-1)];

            avg /= 4.0;
            m_heightMap[x][y] = avg + dRand(m_range);

            if(x == 0) m_heightMap[m_heightMapSize-1][y] = avg;
            if(y == 0) m_heightMap[x][m_heightMapSize-1] = avg;
        }
    }
}


sf::Texture& Map::getTileTexture(const int cellHeight) const
{
    if( cellHeight == 0)
    {
        return *m_engine->m_texmng->getTextureRef("bg_deepwater");
    }
    else if( cellHeight == 1)
    {
        return *m_engine->m_texmng->getTextureRef("bg_water");
    }
    else if(cellHeight == 2)
    {
        return *m_engine->m_texmng->getTextureRef("bg_sand");
    }
    else if(cellHeight == 3)
    {
        return *m_engine->m_texmng->getTextureRef("bg_plain");
    }
    else if(cellHeight == 4)
    {
        return *m_engine->m_texmng->getTextureRef("bg_forest");
    }
    else if(cellHeight == 5)
    {
        return *m_engine->m_texmng->getTextureRef("bg_stone1");
    }
    else if(cellHeight == 6)
    {
        return *m_engine->m_texmng->getTextureRef("bg_stone2");
    }
    else if(cellHeight == 7)
    {
        return *m_engine->m_texmng->getTextureRef("bg_snow");
    }

}

rs::TileType Map::getTileType(const int cellHeight) const
{
    if(cellHeight == 0)        return rs::TileType::DEEPWATER;
    else if(cellHeight == 1)   return rs::TileType::WATER;
    else if(cellHeight == 2)    return rs::TileType::SAND;
    else if(cellHeight == 3)    return rs::TileType::PLAIN;
    else if(cellHeight == 4)    return rs::TileType::FOREST;
    else if(cellHeight == 5)    return rs::TileType::STONE;
    else if(cellHeight == 6)    return rs::TileType::ROCKS;
    else if(cellHeight == 7)    return rs::TileType::SNOW;
    else return rs::TileType::VOID;
}

