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
    int getHandValue() const; // Zwraca sumê punktów w rêce gracza
    void reset();             // Resetuje rêkê gracza
};

#endif // PLAYER_H
