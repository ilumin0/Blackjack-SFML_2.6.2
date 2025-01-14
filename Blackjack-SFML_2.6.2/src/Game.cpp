#include "Game.h"
#include "AssetManager.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blackjack"),
    player("Player"),
    dealer(),
    menu("assets/backgrounds/mainmenu.png", "assets/fonts/arial.ttf",{ "Start Game", "Settings", "Exit" }, "assets/backgrounds/settings.png"),
    currentState(State::Menu),
    gameOver(false),
    result(""),
    currentBet(0),
    betPlaced(false), 
    doubleDown(false),
    outOfChips(false),
    numberOfPlayers(1){
    if (!backgroundTexture.loadFromFile("assets/backgrounds/game.png")) {
        throw std::runtime_error("Failed to load game background texture.");
    }
    backgroundSprite.setTexture(backgroundTexture);
    userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";
}

void Game::run() {
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (currentState == State::Menu) {
                handleMenuInput(event);
            }
            else if (currentState == State::Settings) {
                handleSettingsInput(event);
            }
            else if (currentState == State::Playing) {
                if (!betPlaced) {
                    userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Num1) placeBet(1);
                        else if (event.key.code == sf::Keyboard::Num2) placeBet(5);
                        else if (event.key.code == sf::Keyboard::Num3) placeBet(10);
                        else if (event.key.code == sf::Keyboard::Num4) placeBet(25);
                        else if (event.key.code == sf::Keyboard::Num5) placeBet(100);
                        if (betPlaced) {
                            player.hit(deck);
                            player.hit(deck);
                            dealer.hit(deck);
                            dealer.hit(deck);

                            // Sprawdzanie Blackjacka zaraz po rozdaniu
                            if (player.isBlackjack() && dealer.isBlackjack()) {
                                gameOver = true;
                                result = "Draw! Both have Blackjack!";
                                chips.addChips(currentBet);
                                scoreManager.addDraw();
                                userMessage = "Result: " + result + ". Press R to play again.";
                            }
                            else if (player.isBlackjack()) {
                                gameOver = true;
                                result = "You have Blackjack! You won!";
                                chips.addChips(currentBet * 2.5); // Wygrana 3:2 dla Blackjacka
                                scoreManager.addWin();
                                userMessage = "Result: " + result + ". Press R to play again.";
                            }
                            else if (dealer.isBlackjack()) {
                                gameOver = true;
                                result = "Dealer has Blackjack! You lose!";
                                checkWinner();
                                scoreManager.addLoss();
                                userMessage = "Result: " + result + ". Press R to play again.";
                            }
                        }
                    }
                }
                else if (!gameOver && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::H) {
                        player.hit(deck);
                        if (player.isBusted()) {
                            gameOver = true;
                            result = "You lost!";
                            userMessage = "Result: " + result + ". Press R to restart.";
                            checkWinner();
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        currentState = State::RevealDealerCard;
                        userMessage = "Dealer is finishing their turn...";
                    }
                    else if (event.key.code == sf::Keyboard::D && !doubleDown && player.getCards().size() == 2) {
                        if (chips.canBet(currentBet)) {
                            doubleDown = true; // Ustawienie flagi
                            chips.removeChips(currentBet);
                            currentBet *= 2; // Podwojenie zak³adu
                            player.hit(deck); // Gracz dobiera jedn¹ kartê
                            if (player.isBusted()) {
                                gameOver = true;
                                result = "You lost!";
                                userMessage = "Result: " + result + ". Press R to restart.";
                                checkWinner();
                            }
                            else {
                                currentState = State::RevealDealerCard; // Przechodzimy do tury krupiera
                                userMessage = "Dealer is finishing their turn...";
                            }
                        }
                        else {
                            userMessage = "Not enough chips to Double Down!";
                        }
                    }
                }

                if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    // Dopiero teraz sprawdzamy, czy mamy ¿etony
                    if (chips.getTotalChips() <= 0) {
                        // Nie ma ¿etonów - wyœwietlamy komunikat i przechodzimy np. do specjalnego stanu
                        outOfChips = true;
                    }
                    else {
                        // S¹ ¿etony - resetujemy grê normalnie
                        resetGame();
                        userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";
                    }
                }

            }
            else if (currentState == State::RevealDealerCard) {
                dealer.revealFirstCard();
                currentState = State::DealerTurn;
                clock.restart();
            }
        }

        // SPRAWDZENIE, CZY GRACZ MA ¯ETONY
        // ----------------------------
        if (outOfChips) {
            // Ustawiamy userMessage, ¿eby gracz wiedzia³ co siê sta³o
            userMessage = "You are out of chips! Press any key to exit.";

            // Rysujemy jeszcze ostatni raz t³o
            window.clear();
            window.draw(backgroundSprite);

            // Mo¿emy te¿ wyœwietliæ ten komunikat na ekranie:
            sf::Font font;
            if (font.loadFromFile("assets/fonts/arial.ttf")) {
                sf::Text messageText(userMessage, font, 40);
                messageText.setFillColor(sf::Color::Red);
                messageText.setPosition(500, 500);
                window.draw(messageText);
            }
            window.display();

            // Czekamy na naciœniêcie dowolnego klawisza
            bool waitingForKeyPress = true;
            while (waitingForKeyPress) {
                while (window.pollEvent(event)) {
                    // Jeœli klikniêto "close" w oknie
                    if (event.type == sf::Event::Closed) {
                        window.close();
                        return; // Koñczymy run()
                    }
                    // Jeœli naciœniêto dowolny klawisz
                    if (event.type == sf::Event::KeyPressed ||
                        event.type == sf::Event::MouseButtonPressed) {
                        // Zamykamy okno i koñczymy grê
                        window.close();
                        return;
                    }
                }
            }
        }

        if (currentState == State::DealerTurn) {
            if (clock.getElapsedTime().asSeconds() >= 1.0f) {
                if (!dealer.isDone()) {
                    dealer.hit(deck);
                }
                else {
                    currentState = State::Playing;
                    gameOver = true;
                    userMessage = "Result: " + result + ". Press R to play again.";
                    checkWinner();
                }
                clock.restart();
            }
        }

        // Ustawianie komunikatu w zale¿noœci od stanu gry
        if (currentState == State::Playing) {
            if (!betPlaced) {
                userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";
            }
            else if (!gameOver) {
                userMessage = "Press H to Hit, S to Stand";
                if (!doubleDown && player.getCards().size() == 2) {
                    userMessage += ", or D to Double Down.";
                }
            }
            else if (gameOver) {
                userMessage = "Result: " + result + ". Press R to play again.";
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        // --- Rysowanie w zale¿noœci od stanu ---
        if (currentState == State::Menu) {
            menu.drawMainMenu(window);
        }
        else if (currentState == State::Settings) {
            menu.drawSettings(window, numberOfPlayers);
        }
        else if (currentState == State::Playing || currentState == State::RevealDealerCard || currentState == State::DealerTurn) {
            player.drawHand(window);
            chips.draw(window);

            float dealerXOffset = 300.0f;
            for (size_t i = 0; i < dealer.getHand().size(); ++i) {
                sf::Sprite cardSprite;
                if (!gameOver && i == 0 && currentState != State::RevealDealerCard && currentState != State::DealerTurn) {
                    const sf::Texture& reverseTexture = AssetManager::getInstance()->getTexture("assets/cards/reverse.png");
                    cardSprite.setTexture(reverseTexture);
                }
                else {
                    const sf::Texture& cardTexture = AssetManager::getInstance()->getTexture(
                        "assets/cards/" + dealer.getHand()[i].getRankString() + "_of_" + dealer.getHand()[i].getSuitString() + ".png"
                    );
                    cardSprite.setTexture(cardTexture);
                }
                cardSprite.setPosition(dealerXOffset, 100);
                cardSprite.setScale(0.3f, 0.3f);
                window.draw(cardSprite);
                dealerXOffset += 150.0f;
            }

            sf::Font font;
            if (font.loadFromFile("assets/fonts/arial.ttf")) {
                sf::Text scoreText(scoreManager.getScoreString(), font, 30);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(10, 10);
                window.draw(scoreText);

                sf::Text betText("Bet: " + std::to_string(currentBet), font, 30);
                betText.setFillColor(sf::Color::White);
                betText.setPosition(1500, 10);
                window.draw(betText);

                sf::Text messageText(userMessage, font, 30);
                messageText.setFillColor(sf::Color::Yellow);
                messageText.setPosition(10, 50);
                window.draw(messageText);
            }
        }

        window.display();
    }
}

void Game::checkWinner() {
    int playerValue = player.getHandValue();
    int dealerValue = dealer.getHandValue();

    if (playerValue > 21) {
        result = "You lost!";
        scoreManager.addLoss();
    }
    else if (dealerValue > 21) {
        result = "You won!";
        chips.addChips(currentBet * 2);
        scoreManager.addWin();
    }
    else if (playerValue > dealerValue) {
        result = "You won!";
        chips.addChips(currentBet * 2);
        scoreManager.addWin();
    }
    else if (playerValue < dealerValue) {
        result = "You lost!";
        scoreManager.addLoss();
    }
    else {
        result = "Draw!";
        chips.addChips(currentBet);
        scoreManager.addDraw();
    }
}

void Game::resetGame() {
    deck = Deck();
    deck.shuffle();

    player.reset();
    dealer.reset();

    gameOver = false;
    result = "";
    currentBet = 0;
    betPlaced = false;
    doubleDown = false;

    userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";
}

void Game::placeBet(int amount) {
    if (!betPlaced && chips.canBet(amount)) {
        currentBet = amount;
        chips.removeChips(amount);
        betPlaced = true;
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
            // Wracamy do menu g³ównego
            currentState = State::Menu;
        }
    }
}



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
            if (selectedOption == 0) {
                currentState = State::Playing;

                // Nowe rozdanie ¿etonów
                chips = Chips();
                outOfChips = false;
                scoreManager.reset();

                resetGame(); // Resztê stanu gry zresetuj
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
