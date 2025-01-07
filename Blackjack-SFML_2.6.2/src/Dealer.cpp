#include "Dealer.h"

Dealer::Dealer() : Hand() {}

void Dealer::playTurn(Deck& deck) {
    while (getValue() < 17) { // Krupier dobiera karty, jeœli ma mniej ni¿ 17 punktów
        hit(deck);
    }
}

bool Dealer::isDone() const {
    return getValue() >= 17; // Krupier koñczy, gdy ma 17 lub wiêcej punktów
}

void Dealer::revealFirstCard() {
    if (!cards.empty()) {
        cards[0].setRevealed(true);
    }
}

void Dealer::reset() {
    cards.clear(); // Czyœci karty w rêce
}

int Dealer::getHandValue() const {
    return getValue(); // U¿ywamy metody z klasy `Hand`
}

const std::vector<Card>& Dealer::getHand() const {
    return cards; // Zwracamy karty w rêce
}
