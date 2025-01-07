#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include <string>

class Player : public Hand {
private:
    std::string name;

public:
    Player(const std::string& name);
    void drawHand(sf::RenderWindow& window) const;
    bool isBusted() const;
    int getHandValue() const; // Zwraca sum� punkt�w w r�ce gracza
    void reset();             // Resetuje r�k� gracza
};

#endif // PLAYER_H
