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

    int createObject(rs::ObjectType type, sf::Texture* texture, float x, float y){
        m_texture = texture;        

        m_x = x;
        m_y = y;

        m_isSelected = false;

        m_objectType = type;

        return 0;
    }

    virtual void update(const float dt) = 0;
    virtual void draw(sf::RenderWindow *view) = 0;


public:
    bool m_isSelected;

    float m_x;
    float m_y;
    sf::Texture* m_texture;
    sf::Sprite m_sprite;

    rs::ObjectType m_objectType;
};

class DynamicObject: public Object
{
public:
    DynamicObject(Player* player, Map* map, sf::Texture *texture, float x, float y);
    ~DynamicObject();

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow *view);

    void moveTaskSetup(rs::Point start, rs::Point end);
    void addTask(rs::Point task);

    std::vector<rs::Point> m_moveTask;
    int m_cargoLoaded;
    int m_capacity;

private:
    void cargoExchange();

private:

    rs::Resources m_cargoType;
    bool m_isActive;

    Map* m_map;
    Player* m_player;

    float m_speedX;
    float m_speedY;
    float m_acceleration;

    PathFinder* m_finder;

    std::vector<PPoint*>*  m_path;

};

class Industries : public Object
{
public:
    Industries(rs::ObjectType objType, sf::Texture* texture, rs::IndustryType type, float x, float y);
    ~Industries(){}

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow *view); //Возможно прорисовывает несколько раз. Проверить

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

};

class Road : public Object
{
public:
    Road(rs::ObjectType objType,sf::Texture* texture, rs::RoadType type);
    ~Road(){}

    virtual void update(const float dt);
    virtual void draw(sf::RenderWindow *view);

    void setNewType(sf::Texture* texture, rs::RoadType type)
        {m_type = type; updateSprite(texture);}

    rs::RoadType getType(){return m_type;}

private:
    void updateSprite(sf::Texture* texture);

private:
    rs::RoadType m_type;
};












#endif // OBJECT_H
