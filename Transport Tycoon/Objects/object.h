#pragma once

#ifndef OBJECT_H
#define OBJECT_H

//#include "resources.h"
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
	virtual void loadObject(sf::Texture* texture) = 0;


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

class GreeneryObject : public Object
{
public:
	GreeneryObject() { ; }
	GreeneryObject(rs::Greenery structure, float x, float y);
    ~GreeneryObject(){}

    virtual void update(const float dt) override;
    virtual void draw(sf::RenderWindow& view) override; //Возможно прорисовывает несколько раз. Проверить
	virtual void loadObject(sf::Texture* texture) override;

	void loadObject(rs::Greenery greeneryStruct);
	rs::GreeneryType getGreeneryType() { return m_structure.type; }

protected:
	float m_timeSinceLastChange;
	rs::Greenery m_structure;
	int m_spriteNum;
    

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<Object>(*this);

		ar & m_structure.type;
		ar & m_spriteNum;

	}

};

class Industry : public Object
{
public:
	Industry() { ; }
    Industry(rs::ObjectType objType, sf::Texture* texture, rs::IndustryType type, float x, float y);
    ~Industry(){}

    virtual void update(const float dt) override;
    virtual void draw(sf::RenderWindow& view) override; //Возможно прорисовывает несколько раз. Проверить
	virtual void loadObject(sf::Texture* texture) override;

    void setProp(const rs::IndustryType type);
    void setIsActive();

    rs::IndustryType getType(){return m_type;}

    int m_storage;

protected:
	float m_time_elapsed;		// Timer
    rs::IndustryType m_type;
    int m_workSpeed;			// in 30 days

    int m_iResNum;				// Input res
    rs::Resources m_iRsrc[2];

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
    Road(rs::ObjectType objType, sf::Texture* texture, rs::RoadType type, int x, int y);
    ~Road(){}

    virtual void update(const float dt)override;
    virtual void draw(sf::RenderWindow& view)override;
	virtual void loadObject(sf::Texture* texture)override; // loading procedure

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

class DynamicObject : public Object
{
public:
	
	virtual ~DynamicObject();

	virtual void update(const float dt) = 0;
	virtual void draw(sf::RenderWindow& view) = 0;
	virtual void loadObject(sf::Texture * texture) = 0;

	//void loadObject(sf::Texture * texture, Map* map, Player* player);


	// Moving
	void moveTaskSetup(rs::Point start, rs::Point end);
	void addTask(rs::Point task);
	std::vector<rs::Point> m_moveTask;
	

public:
	Map* m_map;

	bool m_isActive;

	float m_x_iso;
	float m_y_iso;

	float m_speedX;
	float m_speedY;
	//float m_acceleration;

	PathFinder* m_finder;
	std::vector<PPoint*>*  m_path;

private:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar & boost::serialization::base_object<Object>(*this);

		//ar & m_x_iso & m_y_iso;
		ar & m_speedX & m_speedY;

		ar & m_moveTask;

		ar & m_isActive;

	}

};

class Vehicle : public DynamicObject
{
public:
	Vehicle() { ; }
	Vehicle(vhs::Vehicle vehStruct, Map* map, float x, float y);
	//Vehicle(const Vehicle &obj);
	//Vehicle(const Vehicle &&obj);
	~Vehicle();

	virtual void update(const float dt) override;
	virtual void draw(sf::RenderWindow& view) override;
	virtual void loadObject(sf::Texture * texture) override { ; }

	void loadObject(vhs::Vehicle vehStruct, Map* map);

	//int loadVehicle(rs::Cargo cargo) { m_cargo.push_back(cargo); return 0; }
	int m_cargoLoaded;

	vhs::enumVehicle getVehicleType() { return m_vehicleInfo.name; }

	vhs::Vehicle m_vehicleInfo;
private:
	void cargoExchange();

	void unload(Industry* obj);
	void load(Industry* obj);


	void updateDirection();

private:
	

	std::string ownerName;

	vhs::Directions m_direction;

	bool m_isBroken;

	std::vector<rs::Cargo> m_cargo;


private:
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{

		// serialize base class information
		ar << boost::serialization::base_object<DynamicObject>(*this);

		ar << m_vehicleInfo.id;
		ar << m_vehicleInfo.name;

		std::string name = m_vehicleInfo.owner->getPlayerName();
		ar << name;
		ar << m_direction;

		ar << m_cargoLoaded;
		ar << m_isBroken;

		ar << m_cargo;

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		// serialize base class information
		ar >> boost::serialization::base_object<DynamicObject>(*this);

		ar >> m_vehicleInfo.id;
		ar >> m_vehicleInfo.name;

		// FIX ME, adding a standart player
		ar >> ownerName;
		ar >> m_direction;

		ar >> m_cargoLoaded;
		ar >> m_isBroken;

		ar >> m_cargo;

	}


};


#endif // OBJECT_H
