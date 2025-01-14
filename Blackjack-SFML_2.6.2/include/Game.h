#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Menu.h"
#include "ScoreManager.h"
#include "Chips.h"

class Game {
private:
    sf::RenderWindow window;
    Deck deck;
    Player player;
    Dealer dealer;
    Menu menu;
    ScoreManager scoreManager;
    Chips chips;
    int currentBet;
    bool betPlaced;
    bool gameOver;
    std::string result;
    std::string userMessage;

    // [ZMIANA] Usuwamy prywatn¹ sf::Texture backgroundTexture i dajemy tylko sprite
    sf::Sprite backgroundSprite;

    enum class State { Menu, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;

    void handleMenuInput(sf::Event& event);
    void handleSettingsInput(sf::Event& event);
    void resetGame();
    void checkWinner();
    void placeBet(int amount);

    bool doubleDown;
    bool outOfChips;

    int numberOfPlayers;

public:
    Game();
    void run();
};

#endif // GAME_H
