#ifndef CHIPS_H
#define CHIPS_H

#include <SFML/Graphics.hpp>
#include <vector>

class Chips {
private:
    int totalChips;                           // ��czna liczba �eton�w
    std::vector<int> chipDenominations;       // Nomina�y �eton�w
    std::vector<sf::Sprite> chipSprites;      // Sprite'y �eton�w

public:
    Chips();
    void addChips(int amount);
    void removeChips(int amount);
    int getTotalChips() const;
    void draw(sf::RenderWindow& window);
    bool canBet(int amount) const;
};

#endif // CHIPS_H
