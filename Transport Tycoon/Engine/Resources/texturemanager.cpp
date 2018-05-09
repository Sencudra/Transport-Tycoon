#include "texturemanager.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>



DataManager::DataManager()
{

}


void DataManager::loadTexture(const std::string& name, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    textures[name] = tex;

    return;
}

void DataManager::loadTexture(const rs::IndustryType type, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    ind_textures[type] = tex;

    return;
}

void DataManager::loadTexture(const rs::RoadType type, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    road_textures[type] = tex;

    return;
}

void DataManager::loadVehicleBase()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("data.ini", pt);

	rs::vhs::Vehicle truck_1;
	truck_1.name			=			  pt.get<std::string>("balogh_truck.name");
	truck_1.price			=	std::stoi(pt.get<std::string>("balogh_truck.price"));
	truck_1.speed			=	std::stoi(pt.get<std::string>("balogh_truck.speed"));
	truck_1.runCost			=	std::stoi(pt.get<std::string>("balogh_truck.run_cost"));
	truck_1.dateDesigned	=	std::stoi(pt.get<std::string>("balogh_truck.date_designed"));
	truck_1.lifespan		=	std::stoi(pt.get<std::string>("balogh_truck.lifespan"));
	truck_1.capacity		=	std::stoi(pt.get<std::string>("balogh_truck.capacity"));
	truck_1.capacity		= std::stoi(pt.get<std::string>("balogh_truck.capacity"));

	this->addToVehiclesBase(rs::vhs::enumVehicle::BALOGH, truck_1);


	rs::vhs::Vehicle truck_2;
	truck_2.name			= pt.get<std::string>("uhl_truck.name");
	truck_2.price			= std::stoi(pt.get<std::string>("uhl_truck.price"));
	truck_2.speed			= std::stoi(pt.get<std::string>("uhl_truck.speed"));
	truck_2.runCost			= std::stoi(pt.get<std::string>("uhl_truck.run_cost"));
	truck_2.dateDesigned	= std::stoi(pt.get<std::string>("uhl_truck.date_designed"));
	truck_2.lifespan		= std::stoi(pt.get<std::string>("uhl_truck.lifespan"));
	truck_2.capacity		= std::stoi(pt.get<std::string>("uhl_truck.capacity"));
	truck_2.resources		=		    pt.get<std::string>("uhl_truck.name");

	this->addToVehiclesBase(rs::vhs::enumVehicle::UHL, truck_1);
	

	
}

void DataManager::addToVehiclesBase(const rs::vhs::enumVehicle name, rs::vhs::Vehicle obj)
{
	vehicles[name] = obj;
}





sf::Texture* DataManager::getTextureRef(const std::string& name)
{
    return &this->textures.at(name);
}

sf::Texture* DataManager::getTextureRef(const rs::IndustryType& type)
{
    return &this->ind_textures.at(type);
}

sf::Texture* DataManager::getTextureRef(const rs::RoadType& type)
{
    return &this->road_textures.at(type);
}

rs::vhs::Vehicle DataManager::getVehicleStruct(const rs::vhs::enumVehicle name) const
{
	return vehicles.at(name);	
}


