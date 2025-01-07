#include "Deck.h"
#include <algorithm>
#include <random>
#include <ctime>

Deck::Deck() {
    for (int suit = 0; suit < 4; ++suit) {
        for (int rank = 0; rank < 13; ++rank) {
            cards.emplace_back(static_cast<Card::Suit>(suit), static_cast<Card::Rank>(rank));
        }
    }
}

void Deck::shuffle() {
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(static_cast<unsigned>(std::time(nullptr))));
}

Card Deck::dealCard() {
    if (cards.empty()) {
        throw std::runtime_error("Deck is empty!");
    }
    Card dealtCard = cards.back();
    cards.pop_back();
    return dealtCard;
}
