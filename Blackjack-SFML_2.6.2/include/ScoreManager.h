#pragma once

#ifndef SCORE_MANAGER_H
#define SCORE_MANAGER_H

#include <string>

class ScoreManager {
private:
    int wins;
    int losses;
    int draws;

public:
    ScoreManager();

    void addWin();
    void addLoss();
    void addDraw();
    void reset();

    int getWins() const;
    int getLosses() const;
    int getDraws() const;

    std::string getScoreString() const;
};

#endif // SCORE_MANAGER_H
