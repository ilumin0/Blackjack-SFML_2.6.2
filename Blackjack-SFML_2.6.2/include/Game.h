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
    Menu menu;                       // Menu g��wne
    ScoreManager scoreManager;       // Mened�er wynik�w
    Chips chips;                     // Klasa obs�uguj�ca �etony
    int currentBet;                  // Obecny zak�ad
    bool betPlaced;                  // Czy zak�ad zosta� postawiony
    bool gameOver;                   // Czy gra jest zako�czona
    std::string result;              // Wynik gry
    std::string userMessage;         // Komunikat dla gracza

    sf::Texture backgroundTexture;   // Tekstura t�a gry
    sf::Sprite backgroundSprite;     // Sprite t�a gry

    enum class State { Menu, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;              // Aktualny stan gry

    void handleMenuInput(sf::Event& event);    // Obs�uga wej�cia w menu
    void handleSettingsInput(sf::Event& event); // Obs�uga wej�cia w ustawieniach
    void resetGame();                          // Resetowanie gry
    void checkWinner();                        // Sprawdzanie wyniku gry
    void placeBet(int amount);                 // Funkcja obs�uguj�ca zak�ady

    bool doubleDown; // Flaga oznaczaj�ca, �e gracz wybra� Double Down
    bool outOfChips;
public:
    Game();          // Konstruktor
    void run();      // Uruchomienie gry
};

#endif // GAME_H
