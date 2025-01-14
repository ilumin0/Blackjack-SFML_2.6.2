#include "AssetManager.h"

// Statyczna definicja instancji
AssetManager* AssetManager::instance = nullptr;

AssetManager* AssetManager::getInstance() {
    if (!instance) {
        instance = new AssetManager();
    }
    return instance;
}

// £adowanie/zwracanie tekstury
const sf::Texture& AssetManager::getTexture(const std::string& filename) {
    // Sprawdzamy, czy tekstura jest ju¿ w mapie
    if (textures.find(filename) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load texture: " + filename);
        }
        textures[filename] = std::move(texture);
    }
    return textures.at(filename);
}

// £adowanie/zwracanie czcionki
const sf::Font& AssetManager::getFont(const std::string& filename) {
    if (fonts.find(filename) == fonts.end()) {
        sf::Font font;
        if (!font.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load font: " + filename);
        }
        fonts[filename] = std::move(font);
    }
    return fonts.at(filename);
}
