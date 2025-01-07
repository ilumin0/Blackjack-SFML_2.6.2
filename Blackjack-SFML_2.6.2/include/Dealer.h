#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"
#include "Deck.h"

class Dealer : public Hand {
public:
    Dealer();

    void playTurn(Deck& deck); // Krupier dobiera karty
    bool isDone() const;       // Sprawdza, czy krupier zako�czy� dobieranie
    void revealFirstCard();    // Ods�ania pierwsz� kart�
    void reset();              // Resetuje r�k� krupiera
    int getHandValue() const;  // Zwraca sum� punkt�w w r�ce krupiera
    const std::vector<Card>& getHand() const; // Zwraca wektor kart w r�ce
};

#endif // DEALER_H
