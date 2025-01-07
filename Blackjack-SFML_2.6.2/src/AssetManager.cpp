#include "AssetManager.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager* AssetManager::getInstance() {
    if (!instance)
        instance = new AssetManager();
    return instance;
}

const sf::Texture& AssetManager::getTexture(const std::string& filename) {
    if (textures.find(filename) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            throw std::runtime_error("Failed to load texture: " + filename);
        }
        textures[filename] = std::move(texture);
    }
    return textures.at(filename);
}
