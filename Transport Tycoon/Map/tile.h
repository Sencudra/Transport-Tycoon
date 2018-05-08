#ifndef TILE_H
#define TILE_H

#include <deque>


#include "resources.h"
//#include "animation_handler.hpp"

class Object;
class Industries;
class Road;

std::string tileTypeToStr(rs::TileType type);

class Tile
{
public:
    Tile();
    Tile(const unsigned int height, sf::Texture& texture,
        const rs::TileType tileType, bool* drawFlag);
	~Tile();

	void deleteObject();

    void draw(int x, int y, sf::RenderWindow& window);
	void partDraw(int x, int y, sf::RenderWindow& window);
    void update();

    bool setObject(Object *obj);

	bool isEmptyStatic() { return m_tileStatObj == nullptr ? true : false; }
	


public:
    //AnimationHandler animHandler;
    sf::Sprite m_sprite; // make a massive for layering ATTENTION ATTENTION
	
	bool* drawFlag;  // 
	bool isSpriteDrawn;

	rs::TileType m_tileType;

    Object* m_tileStatObj;
	bool isMainStatic;

	std::vector<Object*> m_tileDynObj;

    int m_tileHeight;

private:
	// Serialization
	friend class boost::serialization::access;
	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{

		ar << m_tileType;
		ar << m_tileHeight;

		ar.template register_type<Industries>();
		ar.template register_type<Road>();
		ar << m_tileStatObj;

	}
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{

		ar >> m_tileType;
		ar >> m_tileHeight;

		ar.template register_type<Industries>();
		ar.template register_type<Road>();
		ar >> m_tileStatObj;

	}

};



#endif // TILE_H
