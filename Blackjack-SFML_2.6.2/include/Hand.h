#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include "Deck.h"
#include <vector>
#include <SFML/Graphics.hpp>

class Hand {
protected:
    std::vector<Card> cards;

public:
    void addCard(const Card& card);
    void hit(Deck& deck);
    int getValue() const;
    void clear();
    bool isBusted() const;
    bool isBlackjack() const;

    // Rysowanie kart
    void draw(sf::RenderWindow& window, sf::Vector2f position = { 300, 500 }) const;

    const std::vector<Card>& getCards() const;
};

#endif // HAND_H
