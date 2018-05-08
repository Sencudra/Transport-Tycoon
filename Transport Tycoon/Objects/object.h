#ifndef OBJECT_H
#define OBJECT_H

#include "resources.h"
#include <math.h>
#include "map.h"
#include "tile.h"


class PathFinder;
class PPoint;
class Player;

template <class T>
class ObjectStorage
{

public:

    ObjectStorage();
    ObjectStorage(const T& obj);
    ~ObjectStorage();


    void pushBack(const T&);
    void pop();

    int getSize(){return m_size;}

private:
    int m_size;
    T** m_dynamicArray;

};

class Object
{
public:
    virtual ~Object(){}

	int createObject(rs::ObjectType type, sf::Texture* texture, float x, float y);

    virtual void update(const float dt) = 0;
    virtual void draw(sf::RenderWindow& view) = 0;


public:
	rs::ObjectType m_objectType;
	sf::Texture* m_texture;
	sf::Sprite m_sprite;

	int m_x, m_y;
    bool m_isSelected;

private:
	friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
		// When the class Archive corresponds to an output archive, the
		// & operator is defined similar to <<.  Likewise, when the class Archive
		// is a type of input archive the & operator is defined similar to >>.

		ar & m_x;
		ar & m_y;
		ar & m_objectType;

    }
};

class DynamicObject: public Object
{
public:
	DynamicObject();
    DynamicObject(Player* player, Map* map, sf::Texture *texture, float x, float y);
    ~DynamicObject();

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow& view);

	void loadSetup();
    void moveTaskSetup(rs::Point start, rs::Point end);
    void addTask(rs::Point task);

    std::vector<rs::Point> m_moveTask;
    int m_cargoLoaded;
    int m_capacity;
	Map* m_map;
	Player* m_player;

private:
    void cargoExchange();

private:

    rs::Resources m_cargoType;
    bool m_isActive;

	float m_x_iso;//
	float m_y_iso;//

    float m_speedX;//
    float m_speedY;//
    //float m_acceleration;

    PathFinder* m_finder;//
	
    std::vector<PPoint*>*  m_path;//

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<Object>(*this);

		ar & m_x_iso & m_y_iso;
		ar & m_speedX & m_speedY;

		ar & m_moveTask;

		ar & m_isActive;

		ar & m_cargoType;
		ar & m_cargoLoaded;
		ar & m_capacity;

	}

};

class Industries : public Object
{
public:
	Industries() { ; }
    Industries(rs::ObjectType objType, sf::Texture* texture, rs::IndustryType type, float x, float y);
    ~Industries(){}

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow& view); //Возможно прорисовывает несколько раз. Проверить

    void setProp(const rs::IndustryType type);
    void setIsActive();

    rs::IndustryType getType(){return m_type;}

    int m_storage;

protected:
    rs::IndustryType m_type;
    int m_workSpeed;      // in 30 days

    int m_iResNum;
    rs::Resources m_iRsrc[1];

    int m_oResNum;
    rs::Resources m_oRsrc[1];

    //std::vector<rs::Cargo> m_storage;
    bool m_isActive;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<Object>(*this);

		ar & m_isActive;

		ar & m_storage;
		ar & m_workSpeed;
		ar & m_type;

	}

};

class Road : public Object
{
public:
	Road() { ; }
    Road(rs::ObjectType objType,sf::Texture* texture, rs::RoadType type);
    ~Road(){}

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow& view);

    void setNewType(sf::Texture* texture, rs::RoadType type)
        {m_type = type; updateSprite(texture);}

    rs::RoadType getType(){return m_type;}

private:
    void updateSprite(sf::Texture* texture);

private:
    rs::RoadType m_type;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<Object>(*this);
		
		ar & m_type;

	}

};












#endif // OBJECT_H
