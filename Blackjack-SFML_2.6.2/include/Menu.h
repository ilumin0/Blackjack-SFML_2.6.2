#pragma once

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Menu {
private:
    sf::Texture backgroundTexture;      // Tekstura t�a menu
    sf::Sprite backgroundSprite;        // Sprite t�a menu
    sf::Font font;                      // Czcionka do wy�wietlania tekstu
    std::vector<sf::Text> menuOptions;  // Opcje menu
    int selectedIndex;                  // Aktualnie wybrana opcja

public:
    // Konstruktor
    Menu(const std::string& backgroundPath, const std::string& fontPath, const std::vector<std::string>& options);

    // Metody
    void draw(sf::RenderWindow& window);   // Rysowanie menu
    void moveUp();                         // Zmiana zaznaczenia w g�r�
    void moveDown();                       // Zmiana zaznaczenia w d�
    int getSelectedOption() const;         // Pobranie aktualnie wybranej opcji
};

#endif // MENU_H
