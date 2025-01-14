#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    sf::Texture settingsBackgroundTexture;
    sf::Sprite settingsBackgroundSprite;

    sf::Font font;
    std::vector<sf::Text> menuOptions;
    int selectedIndex;

public:
    // Konstruktor
    Menu(const std::string& menuBgPath,
        const std::string& fontPath,
        const std::vector<std::string>& options,
        const std::string& settingsBgPath);

    // Rysowanie g³ównego menu
    void drawMainMenu(sf::RenderWindow& window);

    // Rysowanie ekranu ustawieñ
    void drawSettings(sf::RenderWindow& window, int numberOfPlayers);

    void moveUp();
    void moveDown();
    int getSelectedOption() const;
};

#endif // MENU_H
