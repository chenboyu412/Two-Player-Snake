#include "game.h"
#include <chrono>
#include <iostream>

Game::Game()
    : snake1(nullptr),
      snake2(nullptr),
      maxX(0),
      maxY(0),
      foodX(0),
      foodY(0),
      gameEnded(false),
      gameTerminated(false),
      endMessage(""),
      randomEngine(static_cast<unsigned int>(
          std::chrono::system_clock::now().time_since_epoch().count()))
{
}

Game::~Game()
{
    delete snake1;
    delete snake2;
}

void Game::initialize()
{
    console.init();
    console.getSize(maxX, maxY);

    int snake1X = 2;
    int snake1Y = 2;

    int snake2X = maxX - 3;
    int snake2Y = maxY - 3;

    if (snake2X <= snake1X)
    {
        snake2X = snake1X + 5;
    }

    if (snake2Y <= snake1Y)
    {
        snake2Y = snake1Y + 5;
    }

    snake1 = new Snake(snake1X, snake1Y, RIGHT, '*');
    snake2 = new Snake(snake2X, snake2Y, LEFT, '#');

    snake1->draw(console);
    snake2->draw(console);

    generateFood();
}

void Game::run()
{
    initialize();

    while (!gameEnded)
    {
        auto frameStart = std::chrono::steady_clock::now();

        handleAllInputs();

        if (gameTerminated)
        {
            finishGame("Game terminated.");
            break;
        }

        if (!gameEnded)
        {
            updateBothSnakes();
        }

        auto frameEnd = std::chrono::steady_clock::now();
        int elapsedMs = static_cast<int>(
            std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count());

        console.sleep(FRAME_DELAY_MS - elapsedMs);
    }

    console.showCursor();
    console.moveCursor(0, maxY - 1);
    std::cout << endMessage << std::endl;
}

void Game::handleAllInputs()
{
    int processed = 0;
    int key = console.getKey();

    while (key != Console::ERR_KEY && processed < MAX_INPUTS_PER_FRAME && !gameEnded)
    {
        int playerMoved = handleInput(key);

        // If player 1 presses a player-1 control key,
        // only snake 1 gets the extra movement.
        // If player 2 presses a player-2 control key,
        // only snake 2 gets the extra movement.
        if (!gameTerminated && playerMoved != 0)
        {
            moveOneSnake(playerMoved);
        }

        processed++;
        key = console.getKey();
    }
}

int Game::handleInput(int key)
{
    switch (key)
    {
    case Console::KEY_UP_CODE:
        snake1->setDirection(UP);
        return 1;
    case Console::KEY_DOWN_CODE:
        snake1->setDirection(DOWN);
        return 1;
    case Console::KEY_LEFT_CODE:
        snake1->setDirection(LEFT);
        return 1;
    case Console::KEY_RIGHT_CODE:
        snake1->setDirection(RIGHT);
        return 1;

    case 'w':
    case 'W':
        snake2->setDirection(UP);
        return 2;
    case 's':
    case 'S':
        snake2->setDirection(DOWN);
        return 2;
    case 'a':
    case 'A':
        snake2->setDirection(LEFT);
        return 2;
    case 'd':
    case 'D':
        snake2->setDirection(RIGHT);
        return 2;

    case 27:
    case 'q':
    case 'Q':
        gameTerminated = true;
        return 0;

    default:
        return 0;
    }
}

void Game::moveOneSnake(int playerNumber)
{
    Snake* movingSnake = nullptr;
    Snake* otherSnake = nullptr;

    if (playerNumber == 1)
    {
        movingSnake = snake1;
        otherSnake = snake2;
    }
    else
    {
        movingSnake = snake2;
        otherSnake = snake1;
    }

    int nextX = movingSnake->getNextHeadX();
    int nextY = movingSnake->getNextHeadY();

    if (isOutsideWall(nextX, nextY) || otherSnake->contains(nextX, nextY))
    {
        if (playerNumber == 1)
        {
            finishGame("Player 1 loses! Player 2 wins!");
        }
        else
        {
            finishGame("Player 2 loses! Player 1 wins!");
        }

        return;
    }

    bool eatFood = (nextX == foodX && nextY == foodY);
    movingSnake->move(console, eatFood);

    if (eatFood)
    {
        generateFood();
    }
}

void Game::updateBothSnakes()
{
    int next1X = snake1->getNextHeadX();
    int next1Y = snake1->getNextHeadY();

    int next2X = snake2->getNextHeadX();
    int next2Y = snake2->getNextHeadY();

    bool snake1Lose = false;
    bool snake2Lose = false;

    if (isOutsideWall(next1X, next1Y) || snake2->contains(next1X, next1Y))
    {
        snake1Lose = true;
    }

    if (isOutsideWall(next2X, next2Y) || snake1->contains(next2X, next2Y))
    {
        snake2Lose = true;
    }

    // Two heads move to the same cell.
    if (next1X == next2X && next1Y == next2Y)
    {
        snake1Lose = true;
        snake2Lose = true;
    }

    // The two heads swap positions in the same frame.
    if (next1X == snake2->getHeadX() && next1Y == snake2->getHeadY() &&
        next2X == snake1->getHeadX() && next2Y == snake1->getHeadY())
    {
        snake1Lose = true;
        snake2Lose = true;
    }

    if (snake1Lose && snake2Lose)
    {
        finishGame("Both players lose!");
        return;
    }
    else if (snake1Lose)
    {
        finishGame("Player 1 loses! Player 2 wins!");
        return;
    }
    else if (snake2Lose)
    {
        finishGame("Player 2 loses! Player 1 wins!");
        return;
    }

    bool snake1EatFood = (next1X == foodX && next1Y == foodY);
    bool snake2EatFood = (next2X == foodX && next2Y == foodY);

    snake1->move(console, snake1EatFood);
    snake2->move(console, snake2EatFood);

    if (snake1EatFood || snake2EatFood)
    {
        generateFood();
    }
}

void Game::generateFood()
{
    if (maxX <= 0 || maxY <= 0)
    {
        return;
    }

    std::uniform_int_distribution<int> xDistribution(0, maxX - 1);
    std::uniform_int_distribution<int> yDistribution(0, maxY - 1);

    do
    {
        foodX = xDistribution(randomEngine);
        foodY = yDistribution(randomEngine);
    } while ((snake1 != nullptr && snake1->contains(foodX, foodY)) ||
             (snake2 != nullptr && snake2->contains(foodX, foodY)));

    console.drawChar(foodX, foodY, '$');
}

bool Game::isOutsideWall(int x, int y) const
{
    return x < 0 || x >= maxX || y < 0 || y >= maxY;
}

void Game::finishGame(const std::string& message)
{
    gameEnded = true;
    endMessage = message;
}
