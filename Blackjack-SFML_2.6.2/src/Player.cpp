#include "Player.h"

Player::Player(const std::string& name) : name(name) {}

void Player::drawHand(sf::RenderWindow& window) const {
    draw(window); // Rysujemy karty gracza z domy�ln� pozycj�
}

bool Player::isBusted() const {
    return getValue() > 21; // U�ycie metody z klasy `Hand`
}

int Player::getHandValue() const {
    return getValue(); // U�ywamy metody `getValue` z klasy `Hand`
}

void Player::reset() {
    clear(); // Czy�ci karty w r�ce gracza
}
