#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include "Menu.h"
#include "ScoreManager.h"
#include "Chips.h"
#include <vector>
#include <string>

// -------------------------------------------
// Struktura ��cz�ca Player z jego indywidualnymi polami:
//
//  - player:   obiekt reprezentuj�cy logik�/hand gracza
//  - chips:    pula �eton�w konkretnego gracza
//  - currentBet: obecny zak�ad
//  - doubleDown: czy ten gracz u�y� DoubleDown w danej rundzie
//
struct PlayerInfo {
    Player player;
    Chips chips;
    int currentBet;
    bool doubleDown;
    std::string name;
    bool done;

    PlayerInfo(const std::string& defaultName)
        : player(defaultName), // wewn�trznie klasa Player te� ma name, ale mo�emy to zostawi�
        chips(),
        currentBet(0),
        doubleDown(false),
        name(defaultName),   // <-- przypisujemy domy�ln� nazw�
		done(false)
    {
    }
};

class Game {
private:
    sf::RenderWindow window;
    Deck deck;
    Dealer dealer;
    Menu menu;
    ScoreManager scoreManager;

    // Ka�dy gracz w wektorze ma swoj� pul� chips i flag� doubleDown
    std::vector<PlayerInfo> players;

    // Stan gry
    bool gameOver;
    sf::Sprite backgroundSprite;
    std::string result;
    std::string userMessage;

    enum class State { Menu, EnterNames, Playing, RevealDealerCard, DealerTurn, Settings };
    State currentState;

    // Liczba graczy ustalana w Settings
    int numberOfPlayers;

    // Faza obstawiania (kto teraz obstawia)
    int currentBettingPlayerIndex;
    bool allBetsPlaced;

    // Faza grania (kto ma tur� Hit/Stand/DoubleDown)
    int currentPlayerIndex;

    // Zmienne potrzebne do wprowadzania nazw graczy:
    int currentNamingPlayerIndex;    // Kt�ry gracz aktualnie wpisuje
    std::string currentNameInput;    // Bufor wpisywanego tekstu


    // =========================
    // =  Metody pomocnicze   =
    // =========================

    void handleMenuInput(sf::Event& event);
    void handleSettingsInput(sf::Event& event);

    void resetGame();
    void checkWinners();      // Sprawdzenie wynik�w rundy
    void nextPlayer();        // Przej�cie do kolejnego gracza w fazie grania
    bool allPlayersDone() const;

    void placeBet(int amount); // Obstawia aktualny gracz
    void dealInitialCards();   // Rozdajemy karty

    void initPlayers();

    void handleEnterNamesInput(sf::Event& event);
    void drawEnterNames(sf::RenderWindow& window);



public:
    Game();
    void run();
};

#endif // GAME_H