#ifndef GAME_H
#define GAME_H

#include "console.h"
#include "snake.h"
#include <random>
#include <string>

class Game
{
private:
    Console console;
    Snake* snake1;
    Snake* snake2;

    int maxX;
    int maxY;
    int foodX;
    int foodY;

    bool gameEnded;
    bool gameTerminated;

    std::string endMessage;
    std::mt19937 randomEngine;

    static const int FRAME_DELAY_MS = 200;
    static const int MAX_INPUTS_PER_FRAME = 20;

    void initialize();

    // Returns 0 for no movement, 1 for player 1 movement, 2 for player 2 movement.
    int handleInput(int key);
    void handleAllInputs();

    void updateBothSnakes();
    void moveOneSnake(int playerNumber);

    void generateFood();
    bool isOutsideWall(int x, int y) const;
    void finishGame(const std::string& message);

public:
    Game();
    ~Game();

    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;

    void run();
};

#endif
