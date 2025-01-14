#include "Menu.h"
#include "AssetManager.h"
#include <stdexcept>

Menu::Menu(const std::string& menuBgPath,
    const std::string& fontPath,
    const std::vector<std::string>& options,
    const std::string& settingsBgPath)
    : selectedIndex(0)
{
    // 1. Tło menu głównego
    const sf::Texture& bgTexture = AssetManager::getInstance()->getTexture(menuBgPath);
    backgroundSprite.setTexture(bgTexture);

    // 2. Tło ekranu ustawień
    const sf::Texture& settingsTexture = AssetManager::getInstance()->getTexture(settingsBgPath);
    settingsBackgroundSprite.setTexture(settingsTexture);

    // 3. Pobieramy czcionkę (zwraca const sf::Font&)
    const sf::Font& menuFont = AssetManager::getInstance()->getFont(fontPath);

    // 4. Tworzymy sf::Text dla każdej opcji menu
    for (size_t i = 0; i < options.size(); ++i) {
        sf::Text text;
        text.setFont(menuFont);    // → setFont(const sf::Font&)
        text.setString(options[i]);
        text.setCharacterSize(50);

        text.setFillColor(i == 0 ? sf::Color::Red : sf::Color::White);
        text.setPosition(800.f, 400.f + static_cast<float>(i * 80));

        menuOptions.push_back(text);
    }
}

void Menu::drawMainMenu(sf::RenderWindow& window) {
    window.draw(backgroundSprite);

    for (auto& option : menuOptions) {
        window.draw(option);
    }
}

void Menu::drawSettings(sf::RenderWindow& window, int numberOfPlayers) {
    // Rysujemy tło ekranu ustawień
    window.draw(settingsBackgroundSprite);

    // Pobieramy font z 1. opcji (zwraca const sf::Font&)
    const sf::Font& usedFont = *menuOptions[0].getFont();


    // Tworzymy tytuł – przekazujemy referencję, nie wskaźnik
    sf::Text title("SETTINGS", usedFont, 60);
    title.setFillColor(sf::Color::White);
    title.setStyle(sf::Text::Bold);
    title.setPosition(700.f, 100.f);
    window.draw(title);

    // Tekst z liczbą graczy
    sf::Text settingsText;
    settingsText.setFont(usedFont);
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
