#ifndef VEHICLE_H
#define VEHICLE_H

#include <vector>
#include "resources.h"

class Vehicle
{
public:
    Vehicle();
    Vehicle(const Vehicle &obj);
    Vehicle(const Vehicle &&obj);
    ~Vehicle();

    int loadVehicle(rs::Cargo cargo){ m_cargo.push_back(cargo); return 0;}
    int move(float dx,float dy);

private:
    float m_xPos;
    float m_yPos;

    int m_speed;
    bool m_isBroken;
    bool m_isActive;

    std::vector<rs::Cargo> m_cargo;

};

#endif // VEHICLE_H
