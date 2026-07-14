#include "snake.h"
#include "console.h"

Snake::Snake(int startX, int startY, Direction startDirection, char drawSymbol)
    : head(nullptr),
      tail(nullptr),
      direction(startDirection),
      symbol(drawSymbol),
      length(0)
{
    insertAtHead(startX, startY);
}

Snake::~Snake()
{
    while (head != nullptr)
    {
        removeTail();
    }
}

void Snake::setDirection(Direction newDirection)
{
    // The assignment does not forbid turning back directly,
    // so left/right/up/down changes are all accepted.
    direction = newDirection;
}

void Snake::insertAtHead(int x, int y)
{
    Node* newHead = new Node;
    newHead->x = x;
    newHead->y = y;
    newHead->next = head;

    head = newHead;

    if (tail == nullptr)
    {
        tail = newHead;
    }

    length++;
}

void Snake::removeTail()
{
    if (head == nullptr)
    {
        return;
    }

    if (head == tail)
    {
        delete head;
        head = nullptr;
        tail = nullptr;
        length = 0;
        return;
    }

    Node* current = head;

    while (current->next != tail)
    {
        current = current->next;
    }

    delete tail;
    tail = current;
    tail->next = nullptr;
    length--;
}

int Snake::getHeadX() const
{
    return head->x;
}

int Snake::getHeadY() const
{
    return head->y;
}

int Snake::getNextHeadX() const
{
    int x = getHeadX();

    if (direction == LEFT)
    {
        x--;
    }
    else if (direction == RIGHT)
    {
        x++;
    }

    return x;
}

int Snake::getNextHeadY() const
{
    int y = getHeadY();

    if (direction == UP)
    {
        y--;
    }
    else if (direction == DOWN)
    {
        y++;
    }

    return y;
}

bool Snake::contains(int x, int y) const
{
    Node* current = head;

    while (current != nullptr)
    {
        if (current->x == x && current->y == y)
        {
            return true;
        }

        current = current->next;
    }

    return false;
}

void Snake::draw(const Console& console) const
{
    Node* current = head;

    while (current != nullptr)
    {
        console.drawChar(current->x, current->y, symbol);
        current = current->next;
    }
}

void Snake::move(const Console& console, bool grow)
{
    int newX = getNextHeadX();
    int newY = getNextHeadY();

    int oldTailX = tail->x;
    int oldTailY = tail->y;

    insertAtHead(newX, newY);

    if (!grow)
    {
        bool newHeadIsOnOldTail = (newX == oldTailX && newY == oldTailY);

        removeTail();

        if (!newHeadIsOnOldTail)
        {
            console.drawChar(oldTailX, oldTailY, ' ');
        }
    }

    console.drawChar(newX, newY, symbol);
}
