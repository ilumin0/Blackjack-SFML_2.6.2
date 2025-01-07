#pragma once

#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>

class ScoreManager {
private:
    int wins;   // Liczba wygranych
    int losses; // Liczba przegranych
    int draws;  // Liczba remis�w

public:
    // Konstruktor
    ScoreManager();

    // Metody zarz�dzania wynikami
    void addWin();
    void addLoss();
    void addDraw();
    void reset();

    // Gettery
    int getWins() const;
    int getLosses() const;
    int getDraws() const;

    // Wy�wietlanie wynik�w
    std::string getScoreString() const;
};

#endif // SCORE_MANAGER_H
