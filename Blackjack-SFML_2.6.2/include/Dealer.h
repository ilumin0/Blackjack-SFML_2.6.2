#ifndef DEALER_H
#define DEALER_H

#include "Hand.h"
#include "Deck.h"

class Dealer : public Hand {
public:
    Dealer();

    void playTurn(Deck& deck);
    bool isDone() const;
    void revealFirstCard();
    void reset();
    int getHandValue() const;
    const std::vector<Card>& getHand() const;
};

#endif // DEALER_H
