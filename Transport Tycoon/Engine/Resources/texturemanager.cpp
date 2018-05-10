#include "resources.h"
#include "texturemanager.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace rs {
	Resources strToResource(std::string str)
	{
		if (str == "coal")					return Resources::COAL;
		else if (str == "ironore")				return Resources::IRONORE;
		else if (str == "grain")				return Resources::GRAIN;
		else if (str == "livestock")			return Resources::LIVESTOCK;
		else if (str == "steel ")				return Resources::STEEL;
		else if (str == "wood")					return Resources::WOOD;
		else if (str == "oil")					return Resources::OIL;
		else if (str == "goods")				return Resources::GOODS;
		else if (str == "passengers")			return Resources::PASSANGERS;
		else if (str == "mail")					return Resources::MAIL;
		else if (str == "valuables")			return Resources::VALUABLES;
		else std::cout << "ERROR: Resources strToResource - enum not found" << std::endl;
	}


}

namespace vhs
{
	enumVehicle strToName(std::string str)
	{
		if (str == "balogh")				return enumVehicle::BALOGH;
		else if (str == "uhl")				return enumVehicle::UHL;
		else if (str == "dw")				return enumVehicle::DW;
		else std::cout << "ERROR: enumVehicle strToName - enum not found" << std::endl;
	}
}



DataManager::DataManager()
{
	loadSpiteSheet();
	loadVehicleSprites();
	loadVehicleBase();
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

void DataManager::loadTexture(const rs::RoadType type, sf::Texture texture)
{

	/* Add it to the list of textures */
	road_textures[type] = texture;

	return;
}

void DataManager::loadTexture(const vhs::enumVehicle vehicle, const std::map<vhs::Directions, sf::Texture> sprites)
{
	vehicleSprites[vehicle] = sprites;

}


void DataManager::loadSpiteSheet()
{
	/* Load spritesheets available */
	sf::Texture tex;
	sf::IntRect shapeRectangle;
	std::string filename = "media/textures/roads/roadSpriteSheet.png";

	int column_1 = 0;
	int column_2 = 64;
	int column_3 = 128;

	int row_1 = 0;
	int row_2 = 32;
	int row_3 = 64;
	int row_4 = 96;
	int row_5 = 128;
	int row_6 = 160;

	shapeRectangle = sf::IntRect(column_1, row_6, 64, 32);
	tex.loadFromFile(filename, shapeRectangle); this->loadTexture(rs::RoadType::ROAD_0_PATH1, tex);
	
	shapeRectangle = sf::IntRect(column_3, row_5, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_1_PATH1, tex);

	shapeRectangle = sf::IntRect(column_1, row_5, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_1_PATH2, tex);

	shapeRectangle = sf::IntRect(column_2, row_5, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_1_PATH3, tex);

	shapeRectangle = sf::IntRect(column_3, row_4, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_1_PATH4, tex);

	shapeRectangle = sf::IntRect(column_2, row_1, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH1, tex);

	shapeRectangle = sf::IntRect(column_1, row_1, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH2, tex);

	shapeRectangle = sf::IntRect(column_2, row_2, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH3, tex);

	shapeRectangle = sf::IntRect(column_3, row_2, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH4, tex);

	shapeRectangle = sf::IntRect(column_1, row_2, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH5, tex);

	shapeRectangle = sf::IntRect(column_3, row_1, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_2_PATH6, tex);

	shapeRectangle = sf::IntRect(column_1, row_3, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_3_PATH1, tex);

	shapeRectangle = sf::IntRect(column_2, row_3, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_3_PATH2, tex);

	shapeRectangle = sf::IntRect(column_3, row_3, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_3_PATH3, tex);

	shapeRectangle = sf::IntRect(column_1, row_4, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_3_PATH4, tex);

	shapeRectangle = sf::IntRect(column_2, row_4, 64, 32);
	tex.loadFromFile(filename, shapeRectangle);this->loadTexture(rs::RoadType::ROAD_4_PATH1, tex);

}

void DataManager::loadVehicleSprites()
{
	sf::Texture tex;
	std::map<vhs::Directions, sf::Texture> vehicle_balogh;
	//TOP_LEFT, BOTTOM_LEFT, BOTTOM_RIGHT, TOP_RIGHT
	tex.loadFromFile("media\textures\vehicles\balogh_car1.png");
	vehicle_balogh[vhs::Directions::TOP_LEFT] = tex;
	tex.loadFromFile("media\textures\vehicles\balogh_car2.png");
	vehicle_balogh[vhs::Directions::BOTTOM_LEFT] = tex;
	tex.loadFromFile("media\textures\vehicles\balogh_car3.png");
	vehicle_balogh[vhs::Directions::BOTTOM_RIGHT] = tex;
	tex.loadFromFile("media\textures\vehicles\balogh_car4.png");
	vehicle_balogh[vhs::Directions::TOP_RIGHT] = tex;

	vehicleSprites[vhs::enumVehicle::BALOGH] = vehicle_balogh;



}

void DataManager::loadVehicleBase()
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini("data.ini", pt);

	vhs::Vehicle truck_1;
	truck_1.id				=	-1;
	truck_1.name			=	vhs::strToName(pt.get<std::string>("balogh_truck.name"));
	truck_1.owner			=	nullptr;
	truck_1.price			=	std::stoi(pt.get<std::string>("balogh_truck.price"));
	truck_1.speed			=	std::stoi(pt.get<std::string>("balogh_truck.speed"));
	truck_1.runCost			=	std::stoi(pt.get<std::string>("balogh_truck.run_cost"));
	truck_1.dateDesigned	=	std::stoi(pt.get<std::string>("balogh_truck.date_designed"));
	truck_1.lifespan		=	std::stoi(pt.get<std::string>("balogh_truck.lifespan"));
	truck_1.capacity		=	std::stoi(pt.get<std::string>("balogh_truck.capacity"));
	truck_1.resource		=	rs::strToResource(pt.get<std::string>("balogh_truck.resources"));
	truck_1.sprites = this->getTextureRef(truck_1.name);
	this->addToVehiclesBase(truck_1.name, truck_1);



	/*vhs::Vehicle truck_2;
	vhs::enumVehicle eVtruck_2 = vhs::enumVehicle::UHL;
	truck_2.name			= pt.get<std::string>("uhl_truck.name");
	truck_2.owner			= nullptr;
	truck_2.price			= std::stoi(pt.get<std::string>("uhl_truck.price"));
	truck_2.speed			= std::stoi(pt.get<std::string>("uhl_truck.speed"));
	truck_2.runCost			= std::stoi(pt.get<std::string>("uhl_truck.run_cost"));
	truck_2.dateDesigned	= std::stoi(pt.get<std::string>("uhl_truck.date_designed"));
	truck_2.lifespan		= std::stoi(pt.get<std::string>("uhl_truck.lifespan"));
	truck_2.capacity		= std::stoi(pt.get<std::string>("uhl_truck.capacity"));
	truck_2.resource		= rs::strToResource(pt.get<std::string>("uhl_truck.resources"));

	this->addToVehiclesBase(vhs::enumVehicle::UHL, truck_2);
	truck_1.sprites = this->getTextureRef(eVtruck_2);*/
		
}

void DataManager::addToVehiclesBase(const vhs::enumVehicle name, vhs::Vehicle obj)
{
	vehicles[name] = obj;
}

std::map<vhs::Directions, sf::Texture>* DataManager::getTextureRef(const vhs::enumVehicle vehicle)
{
	return &this->vehicleSprites.at(vehicle);
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

vhs::Vehicle DataManager::getVehicleStruct(const vhs::enumVehicle name) const
{
	vhs::Vehicle a = vehicles.at(name);
	return a;
}


