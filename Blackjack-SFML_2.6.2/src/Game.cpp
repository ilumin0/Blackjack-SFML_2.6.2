#include "Game.h"
#include "AssetManager.h"

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Blackjack"),
    player("Gracz"),
    dealer(),
    menu("assets/backgrounds/mainmenu.jpg", "assets/fonts/arial.ttf", { "Start Game", "Settings", "Exit" }),
    currentState(State::Menu),
    gameOver(false),
    result("") {
}

void Game::run() {
    sf::Clock clock; // Zegar do zarz¹dzania odstêpami czasowymi

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
                if (!gameOver && event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::H) {
                        player.hit(deck);
                        if (player.isBusted()) {
                            gameOver = true;
                            result = "Dealer Wins";
                            scoreManager.addLoss();
                        }
                    }
                    else if (event.key.code == sf::Keyboard::S) {
                        currentState = State::RevealDealerCard; // Zmieniamy stan gry
                    }
                }

                if (gameOver && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                    resetGame();
                }
            }
            else if (currentState == State::RevealDealerCard) {
                dealer.revealFirstCard();
                currentState = State::DealerTurn;
                clock.restart();
            }
        }

        if (currentState == State::DealerTurn) {
            if (clock.getElapsedTime().asSeconds() >= 1.0f) { // Odstêp czasowy 1 sekunda
                if (!dealer.isDone()) {
                    dealer.hit(deck);
                }
                else {
                    currentState = State::Playing;
                    gameOver = true;
                    checkWinner();
                }
                clock.restart();
            }
        }

        window.clear(sf::Color::Green);

        if (currentState == State::Menu) {
            menu.draw(window);
        }
        else if (currentState == State::Playing || currentState == State::RevealDealerCard || currentState == State::DealerTurn) {
            player.drawHand(window);

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
            }

            if (gameOver) {
                sf::Font font;
                if (font.loadFromFile("assets/fonts/arial.ttf")) {
                    sf::Text resultText(result, font, 50);
                    resultText.setFillColor(sf::Color::White);
                    resultText.setPosition(760, 500);
                    window.draw(resultText);

                    sf::Text restartText("Press R to Restart", font, 30);
                    restartText.setFillColor(sf::Color::Yellow);
                    restartText.setPosition(760, 580);
                    window.draw(restartText);
                }
            }
        }

        window.display();
    }
}

void Game::checkWinner() {
    int playerValue = player.getHandValue();
    int dealerValue = dealer.getHandValue();

    if (playerValue > 21) {
        result = "Dealer Wins";
        scoreManager.addLoss();
    }
    else if (dealerValue > 21) {
        result = "Player Wins";
        scoreManager.addWin();
    }
    else if (playerValue > dealerValue) {
        result = "Player Wins";
        scoreManager.addWin();
    }
    else if (playerValue < dealerValue) {
        result = "Dealer Wins";
        scoreManager.addLoss();
    }
    else {
        result = "Draw";
        scoreManager.addDraw();
    }
}

void Game::resetGame() {
    deck = Deck();
    deck.shuffle();

    player.reset();
    dealer.reset();

    player.hit(deck);
    player.hit(deck);
    dealer.hit(deck);
    dealer.hit(deck);

    gameOver = false;
    result = "";

    dealer.revealFirstCard();
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
