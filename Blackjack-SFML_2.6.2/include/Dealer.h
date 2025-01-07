#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"
#include "Deck.h"

class Dealer : public Hand {
public:
    Dealer();

    void playTurn(Deck& deck); // Krupier dobiera karty
    bool isDone() const;       // Sprawdza, czy krupier zakoñczy³ dobieranie
    void revealFirstCard();    // Ods³ania pierwsz¹ kartê
    void reset();              // Resetuje rêkê krupiera
    int getHandValue() const;  // Zwraca sumê punktów w rêce krupiera
    const std::vector<Card>& getHand() const; // Zwraca wektor kart w rêce
};

#endif // DEALER_H
