#include "Game.h"
#include "AssetManager.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blackjack"),
    player("Player"),
    dealer(),
    menu("assets/backgrounds/mainmenu.jpg", "assets/fonts/arial.ttf", { "Start Game", "Settings", "Exit" }),
    currentState(State::Menu),
    gameOver(false),
    result(""),
    currentBet(0),
    betPlaced(false) {
    if (!backgroundTexture.loadFromFile("assets/backgrounds/game.png")) {
        throw std::runtime_error("Failed to load game background texture.");
    }
    backgroundSprite.setTexture(backgroundTexture);
    userMessage = "Place your bet: press 1, 5, 10, 25, or 100.";
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
            else if (currentState == State::Playing) {
                if (!betPlaced) {
                    userMessage = "Place your bet: press 1, 5, 10, 25, or 100.";
                    if (event.type == sf::Event::KeyPressed) {
                        if (event.key.code == sf::Keyboard::Num1) placeBet(1);
                        else if (event.key.code == sf::Keyboard::Num2) placeBet(5);
                        else if (event.key.code == sf::Keyboard::Num3) placeBet(10);
                        else if (event.key.code == sf::Keyboard::Num4) placeBet(25);
                        else if (event.key.code == sf::Keyboard::Num5) placeBet(100);
                        if (betPlaced) {
                            userMessage = "Press H to hit or S to stand.";
                            player.hit(deck);
                            player.hit(deck);
                            dealer.hit(deck);
                            dealer.hit(deck);
                        }
                    }
                }
                else if (!gameOver && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::H) {
                        player.hit(deck);
                        if (player.isBusted()) {
                            gameOver = true;
                            userMessage = "You lost! Press R to restart.";
                            scoreManager.addLoss();
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        currentState = State::RevealDealerCard;
                        userMessage = "Dealer is finishing their turn...";
                    }
                }

                if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    resetGame();
                    userMessage = "Place your bet: press 1, 5, 10, 25, or 100.";
                }
            }
            else if (currentState == State::RevealDealerCard) {
                dealer.revealFirstCard();
                currentState = State::DealerTurn;
                clock.restart();
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
                    checkWinner();
                    userMessage = "Result: " + result + ". Press R to play again.";
                }
                clock.restart();
            }
        }

        window.clear();
        window.draw(backgroundSprite);

        if (currentState == State::Menu) {
            menu.draw(window);
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

    userMessage = "Place your bet: press 1, 5, 10, 25, or 100.";
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
        if (event.key.code == sf::Keyboard::Escape) {
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
