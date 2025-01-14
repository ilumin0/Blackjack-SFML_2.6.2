#ifndef CHIPS_H
#define CHIPS_H

#include <SFML/Graphics.hpp>
#include <vector>

class Chips {
private:
    int totalChips;                           // £¹czna liczba ¿etonów
    std::vector<int> chipDenominations;       // Nomina³y ¿etonów
    std::vector<sf::Sprite> chipSprites;      // Sprite'y ¿etonów

public:
    Chips();
    void addChips(int amount);
    void removeChips(int amount);
    int getTotalChips() const;
    void draw(sf::RenderWindow& window);
    bool canBet(int amount) const;
};

#endif // CHIPS_H
