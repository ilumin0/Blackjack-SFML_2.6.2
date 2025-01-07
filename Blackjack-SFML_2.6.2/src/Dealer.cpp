#include "Dealer.h"

Dealer::Dealer() : Hand() {}

void Dealer::playTurn(Deck& deck) {
    while (getValue() < 17) { // Krupier dobiera karty, je�li ma mniej ni� 17 punkt�w
        hit(deck);
    }
}

bool Dealer::isDone() const {
    return getValue() >= 17; // Krupier ko�czy, gdy ma 17 lub wi�cej punkt�w
}

void Dealer::revealFirstCard() {
    if (!cards.empty()) {
        cards[0].setRevealed(true);
    }
}

void Dealer::reset() {
    cards.clear(); // Czy�ci karty w r�ce
}

int Dealer::getHandValue() const {
    return getValue(); // U�ywamy metody z klasy `Hand`
}

const std::vector<Card>& Dealer::getHand() const {
    return cards; // Zwracamy karty w r�ce
}
