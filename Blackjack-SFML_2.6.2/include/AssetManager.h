#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>

class AssetManager {
private:
    static AssetManager* instance;
    std::map<std::string, sf::Texture> textures;

    AssetManager() {}

public:
    static AssetManager* getInstance();
    const sf::Texture& getTexture(const std::string& filename);
};

#endif // ASSETMANAGER_H
