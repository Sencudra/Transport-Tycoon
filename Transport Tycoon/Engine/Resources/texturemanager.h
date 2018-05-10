#pragma once

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H


#include "resources.h"



class DataManager
{
public:
    DataManager();
    void loadTexture(const std::string& name, const std::string& filename);
    void loadTexture(const rs::IndustryType type, const std::string& filename);
    void loadTexture(const rs::RoadType type, const std::string& filename);
	
	void loadVehicleBase();
	void addToVehiclesBase(const vhs::enumVehicle name, vhs::Vehicle obj);



    sf::Texture* getTextureRef(const std::string& name);
    sf::Texture* getTextureRef(const rs::IndustryType& type);
    sf::Texture* getTextureRef(const rs::RoadType& type);
	vhs::Vehicle getVehicleStruct(const vhs::enumVehicle) const;


private:
    std::map<std::string, sf::Texture> textures;
    std::map<rs::IndustryType, sf::Texture> ind_textures;
    std::map<rs::RoadType, sf::Texture> road_textures;
	
	std::map<vhs::enumVehicle, vhs::Vehicle> vehicles;


};

#endif // TEXTUREMANAGER_H
