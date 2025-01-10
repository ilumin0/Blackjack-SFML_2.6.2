#include "Hand.h"

void Hand::addCard(const Card& card) {
    cards.push_back(card);
}

void Hand::hit(Deck& deck) {
    addCard(deck.dealCard());
}

int Hand::getValue() const {
    int value = 0;
    int aces = 0;

    for (const auto& card : cards) {
        value += card.getValue();
        if (card.getRank() == Card::Rank::Ace) {
            ++aces;
        }
    }

    while (value > 21 && aces > 0) {
        value -= 10;
        --aces;
    }

    return value;
}

void Hand::clear() {
    cards.clear();
}

bool Hand::isBusted() const {
    return getValue() > 21;
}

bool Hand::isBlackjack() const {
    return cards.size() == 2 && getValue() == 21;
}


void Hand::draw(sf::RenderWindow& window, sf::Vector2f position) const {
    float offsetX = position.x;
    for (const auto& card : cards) {
        card.draw(window, { offsetX, position.y });
        offsetX += 150.0f; // Odstêp miêdzy kartami
    }
}

const std::vector<Card>& Hand::getCards() const {
    return cards;
}
