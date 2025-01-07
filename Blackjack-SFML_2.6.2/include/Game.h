#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Menu.h"
#include "ScoreManager.h"

class Game {
private:
    sf::RenderWindow window;
    Deck deck;
    Player player;
    Dealer dealer;
    Menu menu;
    ScoreManager scoreManager;
    bool gameOver;
    std::string result;

    sf::Clock clock; // Zegar do zarz¹dzania odstêpami czasowymi

    enum class State { Menu, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;

    void handleMenuInput(sf::Event& event);
    void handleSettingsInput(sf::Event& event);
    void resetGame();
    void checkWinner();

public:
    Game();
    void run();
};

#endif // GAME_H
