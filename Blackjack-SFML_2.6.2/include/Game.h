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
// Struktura ³¹cz¹ca Player z jego indywidualnymi polami:
//
//  - player:   obiekt reprezentuj¹cy logikê/hand gracza
//  - chips:    pula ¿etonów konkretnego gracza
//  - currentBet: obecny zak³ad
//  - doubleDown: czy ten gracz u¿y³ DoubleDown w danej rundzie
//
struct PlayerInfo {
    Player player;
    Chips chips;
    int currentBet;
    bool doubleDown;
    std::string name;
    bool done;

    PlayerInfo(const std::string& defaultName)
        : player(defaultName), // wewnêtrznie klasa Player te¿ ma name, ale mo¿emy to zostawiæ
        chips(),
        currentBet(0),
        doubleDown(false),
        name(defaultName),   // <-- przypisujemy domyœln¹ nazwê
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

    // Ka¿dy gracz w wektorze ma swoj¹ pulê chips i flagê doubleDown
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

    // Faza grania (kto ma turê Hit/Stand/DoubleDown)
    int currentPlayerIndex;

    // Zmienne potrzebne do wprowadzania nazw graczy:
    int currentNamingPlayerIndex;    // Który gracz aktualnie wpisuje
    std::string currentNameInput;    // Bufor wpisywanego tekstu


    // =========================
    // =  Metody pomocnicze   =
    // =========================

    void handleMenuInput(sf::Event& event);
    void handleSettingsInput(sf::Event& event);

    void resetGame();
    void checkWinners();      // Sprawdzenie wyników rundy
    void nextPlayer();        // Przejœcie do kolejnego gracza w fazie grania
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