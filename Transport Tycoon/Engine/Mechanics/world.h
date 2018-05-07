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
	World();
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
	int getDayCount() { return m_day; }

    float m_day;
    Player m_player;

    bool isPause(){return m_isPause;}
	void switchPause();
	void x2Speed();

	bool* getDrawnFlag() { return &m_drawFlag; }

private:

	void drawMap(ScreenView& gameView);

    void updateRoadDirection(int a, int b);


private:

    bool m_isPause;
	bool m_isSpeed;
	bool m_drawFlag; // Tiles draw for one time helper

    ng::ProgramEngine* m_engine;
    ProgramStateMain* m_state;

    std::vector<Object*> m_objStaticContainer;
    std::vector<Object*> m_objDynamContainer;

    Map* m_tileMap;


    // World behaviour

    float m_timePerDay;
    float m_oneDayTimer;


private: 
	// Serialization
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{ 
		// When the class Archive corresponds to an output archive, the
		// & operator is defined similar to <<.  Likewise, when the class Archive
		// is a type of input archive the & operator is defined similar to >>.

		ar & m_day;
		ar & m_player;

		ar & m_isPause;
		ar & m_isSpeed;

		ar.template register_type<Industries>();
		ar.template register_type<Road>();
		ar.template register_type<DynamicObject>();

		ar & m_objDynamContainer;

		ar & m_objStaticContainer;

		ar & m_tileMap;

	}
};

#endif // WORLD_H
