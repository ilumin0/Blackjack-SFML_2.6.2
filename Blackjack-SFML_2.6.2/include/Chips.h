#ifndef CHIPS_H
#define CHIPS_H

#include <SFML/Graphics.hpp>
#include <vector>

class Chips {
private:
    int totalChips;                            // £¹czna liczba ¿etonów
    std::vector<int> chipDenominations;       // Nomina³y ¿etonów
    std::vector<sf::Sprite> chipSprites;      // Sprite'y ¿etonów

public:
    Chips();                                  // Konstruktor
    void addChips(int amount);                // Dodaje ¿etony
    void removeChips(int amount);             // Odejmuje ¿etony
    int getTotalChips() const;                // Zwraca liczbê ¿etonów
    void draw(sf::RenderWindow& window);      // Rysuje ¿etony
    bool canBet(int amount) const;            // Sprawdza, czy mo¿na postawiæ zak³ad
};

#endif // CHIPS_H
