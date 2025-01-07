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
    sf::RenderWindow window;         // Okno gry
    Deck deck;                       // Talia kart
    Player player;                   // Gracz
    Dealer dealer;                   // Krupier
    Menu menu;                       // Menu g��wne
    ScoreManager scoreManager;       // Mened�er wynik�w
    bool gameOver;                   // Czy gra jest zako�czona
    std::string result;              // Wynik gry

    sf::Texture backgroundTexture;   // Tekstura t�a gry
    sf::Sprite backgroundSprite;     // Sprite t�a gry

    sf::Clock clock;                 // Zegar do zarz�dzania odst�pami czasowymi

    enum class State { Menu, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;              // Aktualny stan gry

    void handleMenuInput(sf::Event& event);    // Obs�uga wej�cia w menu
    void handleSettingsInput(sf::Event& event); // Obs�uga wej�cia w ustawieniach
    void resetGame();                          // Resetowanie gry
    void checkWinner();                        // Sprawdzanie wyniku gry

public:
    Game();          // Konstruktor
    void run();      // Uruchomienie gry
};

#endif // GAME_H
