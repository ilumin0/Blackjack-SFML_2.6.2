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
    sf::RenderWindow window;         // Okno gry
    Deck deck;                       // Talia kart
    Player player;                   // Gracz
    Dealer dealer;                   // Krupier
    Menu menu;                       // Menu g³ówne
    ScoreManager scoreManager;       // Mened¿er wyników
    Chips chips;                     // Klasa obs³uguj¹ca ¿etony
    int currentBet;                  // Obecny zak³ad
    bool betPlaced;                  // Czy zak³ad zosta³ postawiony
    bool gameOver;                   // Czy gra jest zakoñczona
    std::string result;              // Wynik gry
    std::string userMessage;         // Komunikat dla gracza

    sf::Texture backgroundTexture;   // Tekstura t³a gry
    sf::Sprite backgroundSprite;     // Sprite t³a gry

    enum class State { Menu, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;              // Aktualny stan gry

    void handleMenuInput(sf::Event& event);    // Obs³uga wejœcia w menu
    void handleSettingsInput(sf::Event& event); // Obs³uga wejœcia w ustawieniach
    void resetGame();                          // Resetowanie gry
    void checkWinner();                        // Sprawdzanie wyniku gry
    void placeBet(int amount);                 // Funkcja obs³uguj¹ca zak³ady

    bool doubleDown; // Flaga oznaczaj¹ca, ¿e gracz wybra³ Double Down
    bool outOfChips;
public:
    Game();          // Konstruktor
    void run();      // Uruchomienie gry
};

#endif // GAME_H
