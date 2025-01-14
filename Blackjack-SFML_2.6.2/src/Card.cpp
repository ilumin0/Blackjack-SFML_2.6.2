#include "Card.h"
#include "AssetManager.h"

Card::Card(Suit suit, Rank rank) : suit(suit), rank(rank), revealed(false) {}

int Card::getValue() const {
    switch (rank) {
    case Rank::Two:    return 2;
    case Rank::Three:  return 3;
    case Rank::Four:   return 4;
    case Rank::Five:   return 5;
    case Rank::Six:    return 6;
    case Rank::Seven:  return 7;
    case Rank::Eight:  return 8;
    case Rank::Nine:   return 9;
    case Rank::Ten:
    case Rank::Jack:
    case Rank::Queen:
    case Rank::King:   return 10;
    case Rank::Ace:    return 11;
    }
    return 0;
}

void Card::setRevealed(bool isRevealed) {
    revealed = isRevealed;
}

bool Card::isRevealed() const {
    return revealed;
}

Card::Suit Card::getSuit() const {
    return suit;
}

Card::Rank Card::getRank() const {
    return rank;
}

void Card::draw(sf::RenderWindow& window, sf::Vector2f position) const {
    const sf::Texture& texture = AssetManager::getInstance()->getTexture(
        "assets/cards/" + getRankString() + "_of_" + getSuitString() + ".png"
    );

    sf::Sprite sprite(texture);
    sprite.setPosition(position);
    sprite.setScale(0.3f, 0.3f);
    window.draw(sprite);
}

std::string Card::getSuitString() const {
    switch (suit) {
    case Suit::Clubs:    return "clubs";
    case Suit::Diamonds: return "diamonds";
    case Suit::Hearts:   return "hearts";
    case Suit::Spades:   return "spades";
    }
    return "";
}

std::string Card::getRankString() const {
    switch (rank) {
    case Rank::Two:   return "2";
    case Rank::Three: return "3";
    case Rank::Four:  return "4";
    case Rank::Five:  return "5";
    case Rank::Six:   return "6";
    case Rank::Seven: return "7";
    case Rank::Eight: return "8";
    case Rank::Nine:  return "9";
    case Rank::Ten:   return "10";
    case Rank::Jack:  return "jack";
    case Rank::Queen: return "queen";
    case Rank::King:  return "king";
    case Rank::Ace:   return "ace";
    }
    return "";
}
