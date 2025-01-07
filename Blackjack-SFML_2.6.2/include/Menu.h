#pragma once

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Texture backgroundTexture;      // Tekstura t³a menu
    sf::Sprite backgroundSprite;        // Sprite t³a menu
    sf::Font font;                      // Czcionka do wyœwietlania tekstu
    std::vector<sf::Text> menuOptions;  // Opcje menu
    int selectedIndex;                  // Aktualnie wybrana opcja

public:
    // Konstruktor
    Menu(const std::string& backgroundPath, const std::string& fontPath, const std::vector<std::string>& options);

    // Metody
    void draw(sf::RenderWindow& window);   // Rysowanie menu
    void moveUp();                         // Zmiana zaznaczenia w górê
    void moveDown();                       // Zmiana zaznaczenia w dó³
    int getSelectedOption() const;         // Pobranie aktualnie wybranej opcji
};

#endif // MENU_H
