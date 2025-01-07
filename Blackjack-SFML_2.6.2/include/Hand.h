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
    void hit(Deck& deck); // Dobieranie karty z talii
    int getValue() const;
    void clear();
    bool isBusted() const; // Sprawdza, czy wartoœæ kart przekracza 21
    void draw(sf::RenderWindow& window, sf::Vector2f position = { 300, 500 }) const; // Dodano domyœln¹ pozycjê
    const std::vector<Card>& getCards() const; // Zwraca wektor kart
};

#endif // HAND_H
