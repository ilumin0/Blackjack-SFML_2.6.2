#include "Menu.h"
#include <stdexcept>

Menu::Menu(const std::string& menuBgPath,
    const std::string& fontPath,
    const std::vector<std::string>& options,
    const std::string& settingsBgPath)
    : selectedIndex(0) // <-- lista inicjalizacyjna pól
{
    // 1. £adowanie t³a menu g³ównego
    if (!backgroundTexture.loadFromFile(menuBgPath)) {
        throw std::runtime_error("Nie mo¿na za³adowaæ t³a menu: " + menuBgPath);
    }
    backgroundSprite.setTexture(backgroundTexture);

    // 2. £adowanie t³a ekranu ustawieñ
    if (!settingsBackgroundTexture.loadFromFile(settingsBgPath)) {
        throw std::runtime_error("Nie mo¿na za³adowaæ t³a settings: " + settingsBgPath);
    }
    settingsBackgroundSprite.setTexture(settingsBackgroundTexture);

    // 3. £adowanie czcionki
    if (!font.loadFromFile(fontPath)) {
        throw std::runtime_error("Nie mo¿na za³adowaæ czcionki: " + fontPath);
    }

    // 4. Tworzenie tekstów opcji menu g³ównego
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(font);
        text.setString(options[i]);
        text.setCharacterSize(50);

        // Pierwsza opcja na czerwono, reszta bia³a
        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);

        // Pozycja (X=800, Y=400 + i*80)
        text.setPosition(800.f, 400.f + static_cast<float>(i * 80));
        menuOptions.push_back(text);
    }
}

// ------------------------------------------------
// Definicje pozosta³ych metod
// ------------------------------------------------

void Menu::drawMainMenu(sf::RenderWindow& window) {
    // Rysujemy t³o menu g³ównego
    window.draw(backgroundSprite);

    // Rysujemy opcje menu
    for (auto& option : menuOptions) {
        window.draw(option);
    }
}

void Menu::drawSettings(sf::RenderWindow& window, int numberOfPlayers) {
    // Rysujemy t³o ekranu ustawieñ
    window.draw(settingsBackgroundSprite);

    // Tytu³
    sf::Text title("SETTINGS", font, 60);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(700.f, 100.f);
    window.draw(title);

    // Tekst z liczb¹ graczy i instrukcjami
    sf::Text settingsText;
    settingsText.setFont(font);
    settingsText.setCharacterSize(40);
    settingsText.setFillColor(sf::Color::White);

    std::string msg =
        "Number of players: " + std::to_string(numberOfPlayers) + "\n\n"
        "Press UP or DOWN to change\n"
        "Press ESC to return to menu";

    settingsText.setString(msg);
    settingsText.setPosition(650.f, 300.f);
    window.draw(settingsText);
}

void Menu::moveUp() {
    if (selectedIndex > 0) {
        menuOptions[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex--;
        menuOptions[selectedIndex].setFillColor(sf::Color::Red);
    }
}

void Menu::moveDown() {
    if (selectedIndex < static_cast<int>(menuOptions.size()) - 1) {
        menuOptions[selectedIndex].setFillColor(sf::Color::White);
        selectedIndex++;
        menuOptions[selectedIndex].setFillColor(sf::Color::Red);
    }
}

int Menu::getSelectedOption() const {
    return selectedIndex;
}
