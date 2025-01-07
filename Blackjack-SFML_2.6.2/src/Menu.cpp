#include "Menu.h"
#include <stdexcept>

// Konstruktor
Menu::Menu(const std::string& backgroundPath, const std::string& fontPath, const std::vector<std::string>& options)
    : selectedIndex(0) {
    // £adowanie tekstury t³a
    if (!backgroundTexture.loadFromFile(backgroundPath)) {
        throw std::runtime_error("Nie mo¿na za³adowaæ t³a: " + backgroundPath);
    }
    backgroundSprite.setTexture(backgroundTexture);

    // £adowanie czcionki
    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Nie mo¿na za³adowaæ czcionki: " + fontPath);
    }

    // Tworzenie opcji menu
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(50);
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White); // Pierwsza opcja jest zaznaczona
        text.setPosition(800, 400 + i * 80); // Pozycja opcji (pionowe odstêpy)
        menuOptions.push_back(text);
    }
}

// Rysowanie menu
void Menu::draw(sf::RenderWindow& window) {
    window.draw(backgroundSprite); // Rysowanie t³a
    for (const auto& option : menuOptions) {
        window.draw(option); // Rysowanie opcji
    }
}

// Zmiana zaznaczenia w górê
void Menu::moveUp() {
    if (selectedIndex > 0) {
        menuOptions[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        menuOptions[selectedIndex].setFillColor(sf::Color::Red);
    }
}

// Zmiana zaznaczenia w dó³
void Menu::moveDown() {
    if (selectedIndex < static_cast<int>(menuOptions.size()) - 1) {
        menuOptions[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        menuOptions[selectedIndex].setFillColor(sf::Color::Red);
    }
}

// Pobranie aktualnie wybranej opcji
int Menu::getSelectedOption() const {
    return selectedIndex;
}
