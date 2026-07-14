#ifndef SNAKE_H
#define SNAKE_H

#include "direction.h"

class Console;

class Snake
{
private:
    struct Node
    {
        int x;
        int y;
        Node* next;
    };

    Node* head;
    Node* tail;
    Direction direction;
    char symbol;
    int length;

    void insertAtHead(int x, int y);
    void removeTail();

public:
    Snake(int startX, int startY, Direction startDirection, char drawSymbol);
    ~Snake();

    Snake(const Snake& other) = delete;
    Snake& operator=(const Snake& other) = delete;

    void setDirection(Direction newDirection);

    int getHeadX() const;
    int getHeadY() const;
    int getNextHeadX() const;
    int getNextHeadY() const;

    bool contains(int x, int y) const;
    void draw(const Console& console) const;
    void move(const Console& console, bool grow);
};

#endif
