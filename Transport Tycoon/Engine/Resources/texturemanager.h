#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H


#include "resources.h"



class TextureManager
{
public:
    TextureManager();
    void loadTexture(const std::string& name, const std::string& filename);
    void loadTexture(const rs::IndustryType type, const std::string& filename);
    void loadTexture(const rs::RoadType type, const std::string& filename);



    sf::Texture* getTextureRef(const std::string& name);
    sf::Texture* getTextureRef(const rs::IndustryType& type);
    sf::Texture* getTextureRef(const rs::RoadType& type);


private:
    std::map<std::string, sf::Texture> textures;
    std::map<rs::IndustryType, sf::Texture> ind_textures;
    std::map<rs::RoadType, sf::Texture> road_textures;


};

#endif // TEXTUREMANAGER_H
