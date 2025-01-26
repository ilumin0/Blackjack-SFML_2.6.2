#include "Game.h"
#include "AssetManager.h"
#include <stdexcept>
#include <sstream>
#include <algorithm> // std::remove_if

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blackjack"),
    dealer(),
    menu("assets/backgrounds/mainmenu.png",
        "assets/fonts/arial.ttf",
        { "Start Game", "Settings", "Exit" },
        "assets/backgrounds/settings.png"),
    currentState(State::Menu),
    gameOver(false),
    result(""),
    numberOfPlayers(1),
    currentBettingPlayerIndex(0),
    allBetsPlaced(false),
    currentPlayerIndex(0),
    currentNamingPlayerIndex(0),
    currentNameInput("")
{
    // Ładujemy tło
    const sf::Texture& bgTexture = AssetManager::getInstance()->getTexture("assets/backgrounds/game.png");
    backgroundSprite.setTexture(bgTexture);

    userMessage = "Place your bet.";
    deck.shuffle();
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            switch (currentState) {
            case State::Menu:
                handleMenuInput(event);
                break;

            case State::Settings:
                handleSettingsInput(event);
                break;
            
            case State::EnterNames:
                handleEnterNamesInput(event);
                break;

            case State::Playing:
                // 1) Faza obstawiania
                if (!allBetsPlaced) {
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Num1) placeBet(1);
                        else if (event.key.code == sf::Keyboard::Num2) placeBet(5);
                        else if (event.key.code == sf::Keyboard::Num3) placeBet(10);
                        else if (event.key.code == sf::Keyboard::Num4) placeBet(25);
                        else if (event.key.code == sf::Keyboard::Num5) placeBet(100);
                    }
                }
                else {
                    // 2) Faza grania (Hit/Stand/DoubleDown)
                    if (!gameOver && event.type == sf::Event::KeyPressed) {
                        PlayerInfo& pInfo = players[currentPlayerIndex];
                        Player& currentPlayer = pInfo.player;

                        if (pInfo.done) {
                            break; // albo return, albo nic nie rób
                        }

                        if (event.key.code == sf::Keyboard::H) {
                            currentPlayer.hit(deck);
                            // Sprawdzamy, czy wartość ręki to 21 lub więcej:
                            if (currentPlayer.isBusted() || currentPlayer.getHandValue() == 21) {
                                pInfo.done = true;  // gracz kończy turę
                                nextPlayer();
                            }
                        }
                        else if (event.key.code == sf::Keyboard::S) {
                            nextPlayer();
                        }
                        else if (event.key.code == sf::Keyboard::D
                            && !pInfo.doubleDown
                            && currentPlayer.getCards().size() == 2)
                        {
                            // DoubleDown dla TEGO gracza
                            int bet = pInfo.currentBet;
                            if (pInfo.chips.canBet(bet)) {
                                pInfo.doubleDown = true;
                                pInfo.chips.removeChips(bet);
                                pInfo.currentBet *= 2;
                                currentPlayer.hit(deck);

                                if (currentPlayer.isBusted()) {
                                    nextPlayer();
                                }
                                else {
                                    nextPlayer();
                                }
                            }
                            else {
                                userMessage = players[currentPlayerIndex].name
                                    + ": not enough chips for DoubleDown!";
                            }
                        }
                    }

                    // Restart rundy (tylko jeśli gameOver)
                    if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                        // Sprawdzamy, czy w ogóle został jakiś gracz
                        if (players.empty()) {
                            // Jeśli nie ma żadnego gracza w wektorze => koniec gry
                            // Wyświetlamy komunikat i czekamy na klawisz
                            // (możesz wstawić okno albo to, co wolisz)
                            window.close();
                        }
                        else {
                            resetGame();
                        }
                    }
                }
                break;

            case State::RevealDealerCard:
                userMessage = "Revealing dealer's first card...";
                dealer.revealFirstCard();
                currentState = State::DealerTurn;
                clock.restart();
                break;

            case State::DealerTurn:

                // Tura krupiera - poniżej w pętli
                break;
            }
        }

        // =========== TURA KRUPIERA ===============
        if (currentState == State::DealerTurn) {
            if (clock.getElapsedTime().asSeconds() >= 1.0f) {
                userMessage = "Dealer's turn. Please wait...";
                if (!dealer.isDone()) {
                    dealer.hit(deck);
                }
                else {
                    // Koniec tury krupiera => sprawdź zwycięzców
                    currentState = State::Playing;
                    gameOver = true;
                    checkWinners();
                    userMessage = result + "  Press R to continue.";
                }
                clock.restart();
            }
        }

        // =========== Gdy allBetsPlaced i nie gameOver, sprawdzamy, czy gracze skończyli ===============
        if (currentState == State::Playing) {
            // 1) Faza obstawiania
            if (!allBetsPlaced) {
                if (currentBettingPlayerIndex < (int)players.size()) {
                    userMessage = players[currentBettingPlayerIndex].name
                        + ", place your bet [1 - 1$, 2 - 5$, 3 - 10$, 4 - 25$, 5 - 100$].";
                }
            }
            else {
                // 2) Faza grania (Hit/Stand/DoubleDown)
                if (!gameOver) {
                    if (currentPlayerIndex < (int)players.size()) {
                        userMessage = players[currentPlayerIndex].name
                            + " turn: [H]it, [S]tand";

                        if (!players[currentPlayerIndex].doubleDown
                            && players[currentPlayerIndex].player.getCards().size() == 2)
                        {
                            userMessage += ", [D]oubleDown";
                        }
                    }
                }
            }
        }


        // =========== RENDEROWANIE ===============
        // =============================
        window.clear();
        window.draw(backgroundSprite);

        // Obsługa menu i ustawień (bez zmian):
        if (currentState == State::Menu) {
            menu.drawMainMenu(window);
        }
        else if (currentState == State::EnterNames) {
            drawEnterNames(window);
        }
        else if (currentState == State::Settings) {
            menu.drawSettings(window, numberOfPlayers);
        }
        else {
            // ---------------------------------------
            // 1. STAŁE DOTYCZĄCE WYŚWIETLANIA KART
            // ---------------------------------------
            // Oryginalne karty: 500×726, skala 0.22 => ~110×160 px

            const float cardScale = 0.22f;
            // Dealer (krupier)
            const float dealerStartX = 200.f;  // <-- ZMIANA: taki sam X jak gracze
            const float dealerStartY = 110.f;  // odpowiednio wyżej, by nie nachodził na komunikaty
            const float dealerCardSpacing = 130.f;  // odstęp między kartami krupiera

            // Gracze w siatce 2 (kolumny) × 2 (wiersze)
            const float playerBaseY = 300.f;  // <-- ZMIANA: wyżej niż poprzednio, by nie ucinało kart
            const float rowSpacing = 300.f;  // <-- ZMIANA: mniejszy odstęp między wierszami
            const float colSpacing = 700.f;
            const float firstColX = 200.f;  // taki sam X co dealerStartX
            const float playerCardSpacing = 130.f;  // odstęp między kartami graczy

            // ============================
            // 2. RYSOWANIE KRUPIERA
            // ============================
            {
                float dealerXOffset = dealerStartX;
                for (size_t i = 0; i < dealer.getHand().size(); ++i) {
                    // Obsługa karty zakrytej / odkrytej
                    const sf::Texture& reverseTexture =
                        AssetManager::getInstance()->getTexture("assets/cards/reverse.png");
                    const sf::Texture& cardTexture =
                        AssetManager::getInstance()->getTexture(
                            "assets/cards/" + dealer.getHand()[i].getRankString() +
                            "_of_" + dealer.getHand()[i].getSuitString() + ".png"
                        );

                    sf::Sprite cardSprite;
                    if (!gameOver && i == 0 &&
                        currentState != State::RevealDealerCard &&
                        currentState != State::DealerTurn)
                    {
                        // Pierwsza karta krupiera zakryta
                        cardSprite.setTexture(reverseTexture);
                    }
                    else {
                        cardSprite.setTexture(cardTexture);
                    }

                    cardSprite.setScale(cardScale, cardScale);
                    cardSprite.setPosition(dealerXOffset, dealerStartY);

                    window.draw(cardSprite);
                    dealerXOffset += dealerCardSpacing;
                }
            }

            // =========================
            // 3. RYSOWANIE GRACZY (2x2)
            // =========================
            for (int i = 0; i < static_cast<int>(players.size()); ++i) {
                // Wyznaczamy wiersz i kolumnę
                int row = i / 2;  // 0 lub 1
                int col = i % 2;  // 0 lub 1

                // Bazowy punkt (x, y) dla gracza i
                float baseX = firstColX + col * colSpacing;
                float baseY = playerBaseY + row * rowSpacing;

                // *** Układ informacji o graczu:
                //    [Player i+1] po lewej, a obok [Bet / Chips] "w drugiej kolumnie"

                const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");

                // Pozycje napisów
                float nameX = baseX;         // lewa "kolumna"
                float infoX = baseX + 180.f; // prawa "kolumna"
                float nameY = baseY;         // "Player i+1"
                float betY = baseY;         // Bet tuż obok w tej samej linii
                float chipsY = baseY + 30.f;  // Chips poniżej Bet

                // Gdzie zaczną się karty tego gracza (nieco niżej):
                float cardsY = baseY + 80.f; // żeby nie nachodziły na napis "Chips"

                // 3.1 Rysowanie nazwy gracza
                sf::Text nameText(players[i].name, font, 25);
                nameText.setFillColor(sf::Color::Cyan);
                nameText.setPosition(nameX, nameY);
                window.draw(nameText);

                // 3.2 Bet
                sf::Text betText("Bet: " + std::to_string(players[i].currentBet), font, 20);
                betText.setFillColor(sf::Color::White);
                betText.setPosition(infoX, betY);
                window.draw(betText);

                // 3.3 Chips
                sf::Text chipsText("Chips: " + std::to_string(players[i].chips.getTotalChips()), font, 20);
                chipsText.setFillColor(sf::Color::Yellow);
                chipsText.setPosition(infoX, chipsY);
                window.draw(chipsText);

                // NOWE: Points
                float pointsX = infoX + 125.f; // tu można dostosować
                float pointsY = betY+ 15.f;          // np. rysujemy w tej samej linii co Bet
                int pointsVal = players[i].player.getHandValue();
                sf::Text pointsText("Points: " + std::to_string(pointsVal), font, 20);
                pointsText.setFillColor(sf::Color::Yellow);
                pointsText.setPosition(pointsX, pointsY);
                window.draw(pointsText);

                // 3.4 Karty gracza
                float offsetX = baseX;
                for (auto& card : players[i].player.getCards()) {
                    const sf::Texture& cardTexture =
                        AssetManager::getInstance()->getTexture(
                            "assets/cards/" + card.getRankString() +
                            "_of_" + card.getSuitString() + ".png"
                        );
                    sf::Sprite sprite(cardTexture);

                    sprite.setScale(cardScale, cardScale);
                    sprite.setPosition(offsetX, cardsY);
                    window.draw(sprite);

                    // Przesuwamy się w prawo o szerokość karty (~110 px) + zapas
                    offsetX += playerCardSpacing;
                }
            }

            // ===============================
            // 4. ScoreManager i userMessage
            // ===============================
            {
                sf::Text scoreText(
                    scoreManager.getScoreString(),
                    AssetManager::getInstance()->getFont("assets/fonts/arial.ttf"),
                    30
                );
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(10.f, 10.f);
                window.draw(scoreText);

                sf::Text messageText(
                    userMessage,
                    AssetManager::getInstance()->getFont("assets/fonts/arial.ttf"),
                    30
                );
                messageText.setFillColor(sf::Color::Yellow);
                messageText.setPosition(10.f, 50.f);
                window.draw(messageText);
            }
        }

        window.display();



    }
}

// ==========================================
// ============= Metody prywatne ===========
// ==========================================

// Reset całej gry / nowej rundy
void Game::resetGame() {
    // Tworzymy nową talię
    deck = Deck();
    deck.shuffle();

    // Czyścimy rękę krupiera
    dealer.reset();

    // Czyścimy (tylko) karty i zakłady graczy:
    for (auto& pInfo : players) {
        pInfo.player.reset();       // czyści karty
        pInfo.currentBet = 0;
        pInfo.doubleDown = false;
        pInfo.done = false;
        // pInfo.chips pozostaje bez zmian
    }

    gameOver = false;
    result.clear();
    allBetsPlaced = false;
    currentBettingPlayerIndex = 0;
    currentPlayerIndex = 0;

    if (!players.empty()) {
        // Bierzemy nazwę pierwszego gracza (indeks 0) 
        userMessage = players[0].name + ", place your bet.";
    }
    else {
        // Jeśli z jakiegoś powodu lista graczy jest pusta, na razie ustaw komunikat pusty
        userMessage = "";
    }
}



void Game::placeBet(int amount) {
    if (currentBettingPlayerIndex >= (int)players.size()) {
        return; // Safety
    }
    PlayerInfo& pInfo = players[currentBettingPlayerIndex];
    // Gracz stawia zakład
    if (pInfo.chips.canBet(amount)) {
        pInfo.chips.removeChips(amount);
        pInfo.currentBet = amount;
        pInfo.doubleDown = false; // reset doubleDown

        currentBettingPlayerIndex++;
        if (currentBettingPlayerIndex >= (int)players.size()) {
            allBetsPlaced = true;
            userMessage = "All bets placed. Dealing cards...";
            dealInitialCards();
        }
        else {
            userMessage = players[currentBettingPlayerIndex].name
                + ", place your bet [1 - 1$, 2 - 5$, 3 - 10$, 4 - 25$, 5 - 100$].";
        }
    }
    else {
        userMessage = players[currentBettingPlayerIndex].name
            + ": Not enough chips to place that bet!";
    }
}

// Rozdajemy karty każdemu + krupierowi
void Game::dealInitialCards() {
    // 1) Gracze
    for (auto& pInfo : players) {
        pInfo.player.hit(deck);
        pInfo.player.hit(deck);

        // Jeśli gracz ma 2-kartowe 21 => done
        if (pInfo.player.isBlackjack()) {
            pInfo.done = true;
        }
    }

    // 2) Dealer
    dealer.hit(deck);
    dealer.hit(deck);

    // 3) Sprawdzenie blackjacka krupiera
    if (dealer.isBlackjack()) {
        // Runda natychmiast się kończy
        // Gracze, którzy też mają blackjack => push (bet wraca)
        // Reszta => przegrywa (bet przepada)
        for (auto& pInfo : players) {
            if (pInfo.player.isBlackjack()) {
                // push
                pInfo.chips.addChips(pInfo.currentBet);
            }
            // else: lose => do nothing
        }

        userMessage = "Dealer has Blackjack! Press R to continue.";
        gameOver = true;
        return;
    }

    // 4) Jeśli dealer nie ma blackjack, przechodzimy do normalnej rozgrywki
    if (!players.empty()) {
        userMessage = players[0].name + ", [H]it, [S]tand, [D]oubleDown.";
    }
    else {
        userMessage = "";
    }
}


// Czy wszyscy gracze skończyli turę (Stand/Bust)?
bool Game::allPlayersDone() const {
    return (currentPlayerIndex >= (int)players.size());
}

// Przechodzimy do kolejnego gracza
void Game::nextPlayer() {
    do {
        currentPlayerIndex++;
    } while (currentPlayerIndex < players.size()
        && players[currentPlayerIndex].done == true);

    // Jeśli wszyscy done albo currentPlayerIndex >= players.size(),
    // przechodzimy do stanu RevealDealerCard (lub DealerTurn).
    if (currentPlayerIndex >= players.size()) {
        currentState = State::RevealDealerCard;
    }
}



// Sprawdzamy zwycięzców
void Game::checkWinners() {
    int dealerValue = dealer.getHandValue();
    bool dealerBust = (dealerValue > 21);
    std::vector<int> blackjacks; // gracze z 2-kartowym 21

    std::vector<int> winners, losers, draws;

    for (int i = 0; i < (int)players.size(); i++) {
        int pValue = players[i].player.getHandValue();

        if (players[i].player.isBusted()) {
            losers.push_back(i);
        }
        else if (dealerBust) {
            winners.push_back(i);
        }
        else {
            if (pValue > dealerValue) {
                winners.push_back(i);
            }
            else if (pValue < dealerValue) {
                losers.push_back(i);
            }
            else {
                draws.push_back(i);
            }
        }
    }

    // 1) WYPŁACANIE WYGRANYCH/REMISÓW
    for (int w : winners) {
        bool hasBJ = (std::find(blackjacks.begin(), blackjacks.end(), w)
            != blackjacks.end());
        if (hasBJ) {
            // 3:2 => 2.5 × stawka
            int bet = players[w].currentBet;
            int payout = bet + (int)(1.5f * bet); // 1.5 stawki zysku + bet
            players[w].chips.addChips(payout);
        }
        else {
            // Normalne 2× stawka
            players[w].chips.addChips(players[w].currentBet * 2);
        }
    }

    // 2) AKTUALIZACJA GLOBALNEGO LICZNIKA
    for (int w : winners) {
        scoreManager.addWin();
    }
    for (int d : draws) {
        scoreManager.addDraw();
    }
    for (int l : losers) {
        scoreManager.addLoss();
    }

    // 3) BUDUJEMY KOMUNIKAT
    std::ostringstream oss;
    if (!winners.empty()) {
        oss << "Won: ";
        for (size_t j = 0; j < winners.size(); ++j) {
            bool bj = (std::find(blackjacks.begin(), blackjacks.end(), winners[j])
                != blackjacks.end());

            oss << players[winners[j]].name;
            if (bj) oss << " (BJ)"; // dopisujemy w nawiasie
            if (j + 1 < winners.size()) oss << ", ";
        }
        oss << "; ";
    }
    if (!draws.empty()) {
        oss << "Draw: ";
        for (size_t j = 0; j < draws.size(); ++j) {
            oss << players[draws[j]].name;
            if (j + 1 < draws.size()) oss << ", ";
        }
        oss << "; ";
    }
    if (!losers.empty()) {
        oss << "Lose: ";
        for (size_t j = 0; j < losers.size(); ++j) {
            oss << players[losers[j]].name;
            if (j + 1 < losers.size()) oss << ", ";
        }
    }
    result = oss.str();

    // 4) Usuwamy graczy z 0 żetonów
    auto it = std::remove_if(players.begin(), players.end(), [](const PlayerInfo& p) {
        return p.chips.getTotalChips() <= 0;
        });
    players.erase(it, players.end());
}


// =============================
// =       Menu/Settings       =
// =============================
void Game::handleMenuInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            menu.moveUp();
        }
        else if (event.key.code == sf::Keyboard::Down) {
            menu.moveDown();
        }
        else if (event.key.code == sf::Keyboard::Enter) {
            int selectedOption = menu.getSelectedOption();

            if (selectedOption == 0) { // "Start Game"
                scoreManager.reset();

                initPlayers();          // tworzy wektor players, np. z domyślnymi "Player 1", "Player 2", ...
                resetGame();            // czyści karty, ustawia stany startowe

                // Przygotowujemy się do wpisywania nazw
                currentNamingPlayerIndex = 0;
                currentNameInput.clear();

                currentState = State::EnterNames;
            }

            else if (selectedOption == 1) {
                currentState = State::Settings;
            }
            else if (selectedOption == 2) {
                window.close();
            }
        }
    }
}

void Game::handleSettingsInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            if (numberOfPlayers < 4) {
                numberOfPlayers++;
            }
        }
        else if (event.key.code == sf::Keyboard::Down) {
            if (numberOfPlayers > 1) {
                numberOfPlayers--;
            }
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            currentState = State::Menu;
        }
    }
}

void Game::initPlayers() {
    // Gdy zaczynamy NOWĄ grę, wypełniamy wektor od zera.
    players.clear();
    for (int i = 0; i < numberOfPlayers; ++i) {
        // PlayerInfo konstruktor ustawia chips=100
        players.emplace_back("Player " + std::to_string(i + 1));
    }
}

void Game::handleEnterNamesInput(sf::Event& event) {
    // Interesuje nas event.type == sf::Event::TextEntered (dla zwykłych znaków)
    // i event.type == sf::Event::KeyPressed (dla Enter, Backspace, itp.)

    if (event.type == sf::Event::TextEntered) {
        // Konwertujemy do "unsigned char"
        unsigned char typedChar = static_cast<unsigned char>(event.text.unicode);

        if (typedChar == 8) {
            // BACKSPACE
            if (!currentNameInput.empty()) {
                currentNameInput.pop_back();
            }
        }
        else if (typedChar >= 32 && typedChar < 127) {
            // Zwykły znak ASCII (spacje, litery, cyfry)
            currentNameInput.push_back(static_cast<char>(typedChar));
        }
        // ignORujemy inne kody (Enter, itp.)
    }
    else if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Enter) {
            // Zatwierdzamy wpisaną nazwę dla bieżącego gracza
            if (currentNamingPlayerIndex < (int)players.size()) {
                players[currentNamingPlayerIndex].name = currentNameInput;
                // W razie potrzeby można też ustawić: 
                // players[currentNamingPlayerIndex].player.setName(currentNameInput);

                currentNameInput.clear(); // czyścimy bufor
                currentNamingPlayerIndex++;

                // Jeśli obsłużyliśmy ostatniego gracza, przechodzimy do normalnej rozgrywki
                if (currentNamingPlayerIndex >= (int)players.size()) {
                    // Mamy nazwy wszystkich -> start gry
                    currentState = State::Playing;
                    // Ewentualnie: userMessage = "Player 1, place your bet.";
                }
            }
        }
        else if (event.key.code == sf::Keyboard::Escape) {
            // Można np. przerwać i wrócić do menu
            currentState = State::Menu;
        }
    }
}

void Game::drawEnterNames(sf::RenderWindow& window) {
    // Możesz narysować tło
    // window.draw(backgroundSprite);

    const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");

    // Tytuł
    sf::Text title("Enter player's names", font, 50);
    title.setFillColor(sf::Color::White);
    title.setPosition(450.f, 100.f);
    window.draw(title);

    // Komunikat, który gracz wpisuje nazwę
    if (currentNamingPlayerIndex < (int)players.size()) {
        std::string prompt = "Player " + std::to_string(currentNamingPlayerIndex + 1)
            + " - type your name and press [Enter]:";
        sf::Text promptText(prompt, font, 30);
        promptText.setFillColor(sf::Color::Yellow);
        promptText.setPosition(300.f, 300.f);
        window.draw(promptText);

        // Wyświetlamy to, co aktualnie wpisano
        sf::Text inputText(currentNameInput, font, 30);
        inputText.setFillColor(sf::Color::Cyan);
        inputText.setPosition(300.f, 350.f);
        window.draw(inputText);

        // Dodatkowa instrukcja
        sf::Text info("[Backspace] to edit, [ESC] to cancel", font, 20);
        info.setFillColor(sf::Color::White);
        info.setPosition(300.f, 400.f);
        window.draw(info);
    }
    else {
        // Wszyscy gracze podali nazwę - np. krótkie info
        sf::Text done("All names entered! Press Enter to start...", font, 30);
        done.setFillColor(sf::Color::Green);
        done.setPosition(300.f, 350.f);
        window.draw(done);
    }
}
