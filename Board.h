#ifndef BOARD_H
#define BOARD_H
#include <iostream>
#include "Striker.h"
#include "Player.h"
#include "Coin.h"
using namespace std;

class Board
{
    public:
        float square1;
        float square2;
        float square3;
        float square4;
        float hole_radius;
        float hole_x;
        float hole_y;
        float inner_circle_rad;
        float outer_circle_rad;
        void createBoard();
        bool checkPocketed(Coin *coin);
        bool checkPocketed(Striker *coin);
        void checkWallCollision(Coin *coin);
        void checkWallCollision(Striker *coin);
        Board();
        void output(int x, int y, float r, float g, float b, void* font, char *string);
        void printScore(Player player1);
        virtual ~Board();
    protected:
    private:
};

#endif // BOARD_H
