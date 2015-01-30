#ifndef STRIKER_H
#define STRIKER_H
#include<utility>
#include "Coin.h"
#include <GL/glut.h>
#include<glut.h>
using namespace std;
class Striker: public Coin
{
    public:
        Striker();
        Striker(float cent_x,float cent_y,float rad,float col_r,float col_g,float col_b,float mas,int points);
        void docketCoin();
        float get_hover_x();
        void set_hover_x(float x);
        float get_hover_y();
        void set_hover_y(float x);
        float get_hover_rot();
        void set_hover_rot(float x);
        float get_power();
        void set_power(float pow);
        void display_power_bar(int inp,int endgame);
        void hover_move(int dir);
        void Striker_move(int dir);
        void Striker_shoot();
        void reset_Striker();
        void output(int x, int y, float r, float g, float b, void* font, char *string);
    protected:
    private:
        float hover_x;
        float hover_y;
        float hover_rot;
        float power;
};

#endif // STRIKER_H
