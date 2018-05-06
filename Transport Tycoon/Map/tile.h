#ifndef TILE_H
#define TILE_H

#include <deque>


#include "resources.h"
//#include "animation_handler.hpp"

class Object;

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
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		// When the class Archive corresponds to an output archive, the
		// & operator is defined similar to <<.  Likewise, when the class Archive
		// is a type of input archive the & operator is defined similar to >>.

		ar & m_tileType;
		if (m_tileStatObj && m_tileStatObj->m_objectType == rs::ObjectType::ROAD) ar & m_tileStatObj;
		ar & m_tileHeight;

	}

};



#endif // TILE_H
