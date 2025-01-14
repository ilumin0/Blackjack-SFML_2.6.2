#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    // T³o g³ówne i t³o ustawieñ
    sf::Sprite backgroundSprite;
    sf::Sprite settingsBackgroundSprite;

    // Opcje menu (ka¿da to sf::Text)
    std::vector<sf::Text> menuOptions;

    // Indeks aktualnie wybranej opcji
    int selectedIndex;

public:
    Menu(const std::string& menuBgPath,
        const std::string& fontPath,
        const std::vector<std::string>& options,
        const std::string& settingsBgPath);

    void drawMainMenu(sf::RenderWindow& window);
    void drawSettings(sf::RenderWindow& window, int numberOfPlayers);

    void moveUp();
    void moveDown();
    int getSelectedOption() const;
};

#endif // MENU_H
