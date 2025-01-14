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
    int getHandValue() const;
    void reset();
};

#endif // PLAYER_H
