#ifndef WORLD_H
#define WORLD_H

#include <iostream>


#include "object.h"
#include "map.h"
#include "player.h"


class Player;
class ProgramStateMain;
class ScreenView;

class World
{
public:
    World(int mode, ng::ProgramEngine* engine, ProgramStateMain* state);
    ~World();

    void update(float dt);
    void draw(ScreenView &gameView);

    void saveToFile();
    void loadFromFile();


    Object* selectObject(sf::Vector2f pos);
    void deleteObject(sf::Vector2f pos);

    void addRoad(float x, float y);
    Object *addVehicle(float x, float y);
    void deleteVec(Object* obj);

    void addObject(Object* obj){m_objStaticContainer.push_back(obj);}


    int getTileMapSize(){return m_tileMap->getMapSize();}
    rs::Rectangle getTileMapEdges(){return m_tileMap->getMapEdges();}

    float m_day;
    Player m_player;

    bool isPause(){return m_isPause;}
    void switchPause(){m_isPause == true ? m_isPause = false : m_isPause = true;}
    void x2Speed(){m_timePerDay == 1.5 ? m_timePerDay = 0.75 : m_timePerDay = 1.5;}

private:
    void drawMap(ScreenView& gameView);

    void updateRoadDirection(int a, int b);


private:

    bool m_isPause;

    ng::ProgramEngine* m_engine;
    ProgramStateMain* m_state;

    std::vector<Object*> m_objStaticContainer;
    std::vector<Object*> m_objDynamContainer;

    Map* m_tileMap;


    // World behaviour

    float m_timePerDay;
    float m_oneDayTimer;

};

#endif // WORLD_H
