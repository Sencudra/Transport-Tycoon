#pragma once 

#ifndef WORLD_H
#define WORLD_H

#include <iostream>

#include "object.h"
#include "map.h"
#include "player.h"


class Player;
class ProgramStateMain;
class ScreenView;
class DynamicObject;

class World
{
public:
	World();
	
    World(ng::ProgramEngine* engine, ProgramStateMain* state);
    ~World();
	void WorldLoadSetup(ng::ProgramEngine* engine, ProgramStateMain* state);
   
	void update(float dt);
    void draw(ScreenView &gameView);

    Object* selectObject(sf::Vector2f pos);
    void deleteObject(sf::Vector2f pos);

    void addRoad(float x, float y);
    Object *addVehicle(float x, float y);
	bool isTileIsRoad(int x, int y);
    void deleteVec(Object* obj);

    void addObject(Object* obj){m_objStaticContainer.push_back(obj);}

    int getTileMapSize(){return m_tileMap->getMapSize();}
    rs::Rectangle getTileMapEdges(){return m_tileMap->getMapEdges();}
	int getDayCount() { return m_day; }

    bool isPause(){return m_isPause;}
	void switchPause();
	void x2Speed();

	bool* getDrawnFlag() { return &m_drawFlag; }


public:
	float m_day;
	Player m_player;
	Map* m_tileMap;

	std::vector<Object*> m_objStaticContainer;
	std::vector<Object*> m_objDynamContainer;

private:

	void drawMap(ScreenView& gameView);

    void updateRoadDirection(int a, int b);


private:

    bool m_isPause;
	bool m_isSpeed;
	bool m_drawFlag; // Tiles draw for one time helper

    ng::ProgramEngine* m_engine;
    ProgramStateMain* m_state;



 
    // World behaviour

    float m_timePerDay;
	float m_oneDayTimer;


private: 
	// Serialization
	friend class boost::serialization::access;

	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{

		ar << m_day;
		ar << m_player;

		ar << m_isPause;
		ar << m_isSpeed;

		ar.template register_type<Industry>();
		ar.template register_type<Road>();
		ar.template register_type<Vehicle>();
		ar.template register_type<GreeneryObject>();

		ar << m_objDynamContainer;

		ar << m_objStaticContainer;

		ar << m_tileMap;

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar >> m_day;
		ar >> m_player;

		ar >> m_isPause;
		ar >> m_isSpeed;

		ar.template register_type<Industry>();
		ar.template register_type<Road>();
		ar.template register_type<Vehicle>();
		ar.template register_type<GreeneryObject>();

		ar >> m_objDynamContainer;
			
		ar >> m_objStaticContainer;

		ar >> m_tileMap;

	}
};

#endif // WORLD_H
