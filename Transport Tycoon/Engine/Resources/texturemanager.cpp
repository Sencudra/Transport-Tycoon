#include "texturemanager.h"

TextureManager::TextureManager()
{

}


void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    textures[name] = tex;

    return;
}

void TextureManager::loadTexture(const rs::IndustryType type, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    ind_textures[type] = tex;

    return;
}

void TextureManager::loadTexture(const rs::RoadType type, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    road_textures[type] = tex;

    return;
}


sf::Texture* TextureManager::getTextureRef(const std::string& name)
{
    return &this->textures.at(name);
}

sf::Texture* TextureManager::getTextureRef(const rs::IndustryType& type)
{
    return &this->ind_textures.at(type);
}

sf::Texture* TextureManager::getTextureRef(const rs::RoadType& type)
{
    return &this->road_textures.at(type);
}
