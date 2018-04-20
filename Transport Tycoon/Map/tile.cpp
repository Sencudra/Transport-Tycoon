#include "tile.h"

#include "object.h"

Tile::Tile()
{

}

Tile::Tile(const unsigned int height, sf::Texture& texture,
    const rs::TileType tileType)
{
    this->m_tileStatObj = nullptr;
	this->isMainStatic = false;

    this->m_tileType = tileType;
    //this->regions[0] = 0;
    this->m_tileHeight = height;

    this->m_sprite.setOrigin(sf::Vector2f(0.0f, 0.0f)); //tileSize*(height-1)));
    this->m_sprite.setTexture(texture);

}

void Tile::draw(int x, int y, sf::RenderWindow& window)
{
	// Draw tiles with/without static/dynamic objects.

    m_sprite.setPosition(x,y);
    window.draw(m_sprite);

	if (this->isMainStatic == true && m_tileStatObj != NULL)
		m_tileStatObj->draw(&window);

	if (m_tileDynObj.size() != 0)
		for (auto i : m_tileDynObj)
		{
			i->draw(&window);
		}
}

void Tile::update()
{

}


bool Tile::setObject(Object* obj)
{
    if(m_tileStatObj == NULL)
    {
        obj->m_sprite.setPosition(this->m_sprite.getPosition().x, this->m_sprite.getPosition().y);
        m_tileStatObj = obj;
        return true;
    }
    else
    {
        return false;
    }
}

void Tile::deleteObject()
{
	if (this->m_tileStatObj == NULL) return;

	if (this->m_tileStatObj->m_objectType == rs::ObjectType::ROAD)
	{
		delete m_tileStatObj;
		m_tileStatObj = NULL;
		bool isMainStatic = false;
	}
}
