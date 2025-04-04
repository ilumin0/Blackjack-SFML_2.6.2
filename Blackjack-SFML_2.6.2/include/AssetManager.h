#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <stdexcept>

class AssetManager {
private:
    static AssetManager* instance;

    // Mapa tekstur
    std::map<std::string, sf::Texture> textures;

    // Mapa czcionek
    std::map<std::string, sf::Font> fonts;

    // Prywatny konstruktor (Singleton)
    AssetManager() {}

public:
    // Pobranie instancji Singletona
    static AssetManager* getInstance();

    // Zarządzanie teksturami
    const sf::Texture& getTexture(const std::string& filename);

    // Zarządzanie czcionkami
    const sf::Font& getFont(const std::string& filename);
};

#endif // ASSETMANAGER_H
