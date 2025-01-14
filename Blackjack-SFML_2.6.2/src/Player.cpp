#include "Player.h"

Player::Player(const std::string& name)
    : name(name)
{
}

void Player::drawHand(sf::RenderWindow& window) const {
    // Karty z domyœln¹ pozycj¹ (300,500)
    draw(window);
}

bool Player::isBusted() const {
    return getValue() > 21;
}

int Player::getHandValue() const {
    return getValue();
}

void Player::reset() {
    clear();
}
