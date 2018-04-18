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
        const rs::TileType tileType);
    ~Tile(){delete m_tileStatObj;}

    void draw(int x, int y, sf::RenderWindow& window);
    void update();

    bool setObject(Object *obj);


public:
    //AnimationHandler animHandler;
    sf::Sprite m_sprite; // make a massive for layering ATTENTION ATTENTION
    rs::TileType m_tileType;
    Object* m_tileStatObj;

    int m_tileHeight;

};










#endif // TILE_H
