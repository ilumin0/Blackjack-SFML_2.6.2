#ifndef CARD_H
#define CARD_H

#include <string>
#include <SFML/Graphics.hpp>

class Card {
public:
    enum class Suit { Clubs, Diamonds, Hearts, Spades };
    enum class Rank { Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King, Ace };

private:
    Suit suit;
    Rank rank;
    bool revealed; // Flaga odkrycia karty

public:
    Card(Suit suit, Rank rank);

    int getValue() const;
    Suit getSuit() const;
    Rank getRank() const;

    void setRevealed(bool isRevealed);
    bool isRevealed() const;

    void draw(sf::RenderWindow& window, sf::Vector2f position) const;

    std::string getSuitString() const;
    std::string getRankString() const;
};

#endif // CARD_H
