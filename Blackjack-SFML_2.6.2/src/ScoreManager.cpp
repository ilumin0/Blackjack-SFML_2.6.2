#include "ScoreManager.h"

// Konstruktor
ScoreManager::ScoreManager() : wins(0), losses(0), draws(0) {}

void ScoreManager::addWin() {
    wins++;
}

void ScoreManager::addLoss() {
    losses++;
}

void ScoreManager::addDraw() {
    draws++;
}

void ScoreManager::reset() {
    wins = 0;
    losses = 0;
    draws = 0;
}

int ScoreManager::getWins() const {
    return wins;
}

int ScoreManager::getLosses() const {
    return losses;
}

int ScoreManager::getDraws() const {
    return draws;
}

std::string ScoreManager::getScoreString() const {
    return "Wins: " + std::to_string(wins) +
        ", Losses: " + std::to_string(losses) +
        ", Draws: " + std::to_string(draws);
}
