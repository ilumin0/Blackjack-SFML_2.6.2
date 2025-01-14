#include "Game.h"
#include "AssetManager.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blackjack"),
    player("Player"),
    dealer(),
    // [ZMIANA] Œcie¿ki do t³a, czcionki i menu przenosimy do param. Menu
    menu("assets/backgrounds/mainmenu.png",
        "assets/fonts/arial.ttf",
        { "Start Game", "Settings", "Exit" },
        "assets/backgrounds/settings.png"),
    currentState(State::Menu),
    gameOver(false),
    result(""),
    currentBet(0),
    betPlaced(false),
    doubleDown(false),
    outOfChips(false),
    numberOfPlayers(1)
{
    // [ZMIANA] T³o gry – pobieramy z AssetManager, przypisujemy do sprite'a
    const sf::Texture& bgTexture = AssetManager::getInstance()->getTexture("assets/backgrounds/game.png");
    backgroundSprite.setTexture(bgTexture);

    userMessage = "Place your bet: press 1, 2, 3, 4, or 5.";

    // Mo¿na ewentualnie dokonaæ pre³adowania zasobów tutaj, jeœli chcemy
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

                            // Sprawdzenie Blackjacka
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
                                chips.addChips(static_cast<int>(currentBet * 2.5f));
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
                            doubleDown = true;
                            chips.removeChips(currentBet);
                            currentBet *= 2;
                            player.hit(deck);
                            if (player.isBusted()) {
                                gameOver = true;
                                result = "You lost!";
                                userMessage = "Result: " + result + ". Press R to restart.";
                                checkWinner();
                            }
                            else {
                                currentState = State::RevealDealerCard;
                                userMessage = "Dealer is finishing their turn...";
                            }
                        }
                        else {
                            userMessage = "Not enough chips to Double Down!";
                        }
                    }
                }

                if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    if (chips.getTotalChips() <= 0) {
                        outOfChips = true;
                    }
                    else {
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

        // Obs³uga stanu, w którym gracz nie ma ¿etonów
        if (outOfChips) {
            userMessage = "You are out of chips! Press any key to exit.";

            window.clear();
            window.draw(backgroundSprite);

            // [ZMIANA] Czcionka z AssetManager
            const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");
            sf::Text messageText(userMessage, font, 40);
            messageText.setFillColor(sf::Color::Red);
            messageText.setPosition(500, 500);
            window.draw(messageText);

            window.display();

            bool waitingForKeyPress = true;
            sf::Event e;
            while (waitingForKeyPress) {
                while (window.pollEvent(e)) {
                    if (e.type == sf::Event::Closed) {
                        window.close();
                        return;
                    }
                    if (e.type == sf::Event::KeyPressed ||
                        e.type == sf::Event::MouseButtonPressed) {
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
            else {
                userMessage = "Result: " + result + ". Press R to play again.";
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        if (currentState == State::Menu) {
            menu.drawMainMenu(window);
        }
        else if (currentState == State::Settings) {
            menu.drawSettings(window, numberOfPlayers);
        }
        else if (currentState == State::Playing ||
            currentState == State::RevealDealerCard ||
            currentState == State::DealerTurn)
        {
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

            const sf::Font& font = AssetManager::getInstance()->getFont("assets/fonts/arial.ttf");

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
                chips = Chips();
                outOfChips = false;
                scoreManager.reset();
                resetGame();
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
