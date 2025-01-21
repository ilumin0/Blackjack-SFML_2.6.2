#include "Chips.h"
#include "AssetManager.h"

Chips::Chips() : totalChips(100) { // Gracz zaczyna ze 100 ¿etonami
    chipDenominations = { 1, 5, 10, 25, 100 };

    for (int denom : chipDenominations) {
        const sf::Texture& texture = AssetManager::getInstance()->getTexture(
            "assets/chips/" + std::to_string(denom) + ".png"
        );
        sf::Sprite sprite(texture);
        sprite.setScale(0.1f, 0.1f); // Skalowanie do odpowiedniego rozmiaru
        chipSprites.push_back(sprite);
    }
}

void Chips::addChips(int amount) {
    totalChips += amount;
}

void Chips::removeChips(int amount) {
    if (amount <= totalChips) {
        totalChips -= amount;
    }
}

int Chips::getTotalChips() const {
    return totalChips;
}

bool Chips::canBet(int amount) const {
    return amount <= totalChips;
}

void Chips::draw(sf::RenderWindow& window) {
    float xOffset = 10.0f;
    float yOffset = 800.0f;

    for (size_t i = 0; i < chipSprites.size(); ++i) {
        sf::Sprite& sprite = chipSprites[i];
        sprite.setPosition(xOffset, yOffset);
        window.draw(sprite);

        // Krzystamy z czcionki AssetManager
        const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");
        sf::Text text(std::to_string(chipDenominations[i]), font, 20);
        text.setFillColor(sf::Color::White);
        text.setPosition(xOffset + 25, yOffset + 50);
        window.draw(text);

        xOffset += 120.0f; // Odstêp miêdzy ¿etonami
    }

    // Wyœwietlanie liczby ¿etonów
    const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");
    sf::Text totalText("Total: " + std::to_string(totalChips), font, 30);
    totalText.setFillColor(sf::Color::Yellow);
    totalText.setPosition(10, 750);
    window.draw(totalText);
}
