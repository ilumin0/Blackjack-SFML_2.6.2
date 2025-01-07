#ifndef CHIPS_H
#define CHIPS_H

#include <SFML/Graphics.hpp>
#include <vector>

class Chips {
private:
    int totalChips;                            // ��czna liczba �eton�w
    std::vector<int> chipDenominations;       // Nomina�y �eton�w
    std::vector<sf::Sprite> chipSprites;      // Sprite'y �eton�w

public:
    Chips();                                  // Konstruktor
    void addChips(int amount);                // Dodaje �etony
    void removeChips(int amount);             // Odejmuje �etony
    int getTotalChips() const;                // Zwraca liczb� �eton�w
    void draw(sf::RenderWindow& window);      // Rysuje �etony
    bool canBet(int amount) const;            // Sprawdza, czy mo�na postawi� zak�ad
};

#endif // CHIPS_H
