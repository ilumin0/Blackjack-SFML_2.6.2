#include "ScoreManager.h"

// Konstruktor
ScoreManager::ScoreManager() : wins(0), losses(0), draws(0) {}

// Dodanie wygranej
void ScoreManager::addWin() {
    wins++;
}

// Dodanie przegranej
void ScoreManager::addLoss() {
    losses++;
}

// Dodanie remisu
void ScoreManager::addDraw() {
    draws++;
}

// Reset wyników
void ScoreManager::reset() {
    wins = 0;
    losses = 0;
    draws = 0;
}

// Pobranie liczby wygranych
int ScoreManager::getWins() const {
    return wins;
}

// Pobranie liczby przegranych
int ScoreManager::getLosses() const {
    return losses;
}

// Pobranie liczby remisów
int ScoreManager::getDraws() const {
    return draws;
}

// Zwraca wyniki jako string
std::string ScoreManager::getScoreString() const {
    return "Wins: " + std::to_string(wins) +
        ", Losses: " + std::to_string(losses) +
        ", Draws: " + std::to_string(draws);
}
