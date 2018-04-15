#include "vehicle.h"


Vehicle::Vehicle()
{
    m_xPos = 0;
    m_yPos = 0;

    m_speed = 60;
    m_isBroken = false;
    m_isActive = true;
}

Vehicle::~Vehicle()
{

}

Vehicle::Vehicle(const Vehicle &obj)
{
    this->m_isBroken = obj.m_isBroken;
    this->m_isActive = obj.m_isActive;
    this->m_speed = obj.m_speed;

    this->m_xPos = 0;
    this->m_yPos = 0;
}

Vehicle::Vehicle(const Vehicle &&obj)
{

}

int Vehicle::move(float dx, float dy)
{
    m_xPos += dx;
    m_yPos += dy;
    return 0;
}

