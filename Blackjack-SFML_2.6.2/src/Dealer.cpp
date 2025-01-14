#include "Dealer.h"

Dealer::Dealer() : Hand() {}

void Dealer::playTurn(Deck& deck) {
    while (getValue() < 17) {
        hit(deck);
    }
}

bool Dealer::isDone() const {
    return getValue() >= 17;
}

void Dealer::revealFirstCard() {
    if (!cards.empty()) {
        cards[0].setRevealed(true);
    }
}

void Dealer::reset() {
    cards.clear();
}

int Dealer::getHandValue() const {
    return getValue();
}

const std::vector<Card>& Dealer::getHand() const {
    return cards;
}
