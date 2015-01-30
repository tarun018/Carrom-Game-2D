#include "Striker.h"
#include "Coin.h"
#include<stdio.h>
#include <string.h>
#include<utility>
#include<iostream>
#include <cmath>
#include <GL/glut.h>
#include<glut.h>
using namespace std;

#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define RAD2DEG(rad) (rad * 180 / PI)
#define ROTATEX(x,y,deg) ((x * cos(DEG2RAD(deg)))-(y*sin(DEG2RAD(deg))))
#define ROTATEY(x,y,deg) ((x * sin(DEG2RAD(deg)))+(y*cos(DEG2RAD(deg))))

float getDistance(float x1,float y1,float x2,float y2);

Striker::Striker()
{
    this->pocketed=0;
    this->position = make_pair(0.0f,0.0f);
    this->radius=0.0f;
    this->mass=0.0f;
    this->velocity = make_pair(0.0f,0.0f);
    this->colour = make_pair(0.0f,make_pair(0.0f,0.0f));
    this->hover_rot = 90.0f;
    this->hover_x = 0.0f;
    this->hover_y = -15.0f;
    this->points=-5;
    this->power = 10.0f;
}

Striker::Striker(float cent_x,float cent_y,float rad,float col_r,float col_g,float col_b,float mas,int points)
{
    this->pocketed=0;
    this->position = make_pair(cent_x,cent_y);
    this->radius=rad;
    this->mass=mas;
    this->velocity = make_pair(0.0f,0.0f);
    this->colour = make_pair(col_r,make_pair(col_g,col_b));
    this->points=points;
    this->hover_rot = 90.0f;
    this->hover_x = 0.0f;
    this->hover_y = -15.0f;
    this->power = 10.0f;
}


void Striker::docketCoin()
{
    this->set_x(-150.0f);
    this->set_y(25.0f);
    this->set_vx(0.0f);
    this->set_vy(0.0f);
}


void Striker::reset_Striker()
{
    this->set_x(0.0f);
    this->set_y(-55.5f);
    this->set_power(10.0f);
    this->set_hover_x(0.0f);
    this->set_hover_y(-15.0f);
    this->set_hover_rot(90.0f);
    this->pocketed = 0;
}

void Striker::hover_move(int dir)
{
    if(dir==1 && hover_rot>=0.0f)
    {
        this->hover_rot -= 1.0f;
        this->hover_x -= this->get_x();
        this->hover_y -= this->get_y();
        this->hover_x = ROTATEX(this->hover_x,this->hover_y,-1.0f);
        this->hover_y = ROTATEY(this->hover_x,this->hover_y,-1.0f);
        this->hover_x += this->get_x();
        this->hover_y += this->get_y();
    }
    else if(dir==-1 && hover_rot<=180.0f)
    {
        this->hover_rot += +1.0f;
        this->hover_x -= this->get_x();
        this->hover_y -= this->get_y();
        this->hover_x = ROTATEX(this->hover_x,this->hover_y,1.0f);
        this->hover_y = ROTATEY(this->hover_x,this->hover_y,1.0f);
        this->hover_x += this->get_x();
        this->hover_y += this->get_y();
    }
}

void Striker::Striker_move(int dir)
{
    if((this->get_x() <= 48.7f && dir==1) || (this->get_x() >= -48.7f && dir==-1))
    {
        this->set_x(this->get_x()+(0.3f*dir));
        this->set_hover_x(this->get_hover_x()+(0.3*dir));
    }
}

void Striker::Striker_shoot()
{
    this->set_vx(((float)(this->get_power()+40)/(float)40)*cos(DEG2RAD(this->get_hover_rot())));
    this->set_vy(((float)(this->get_power()+40)/(float)40)*sin(DEG2RAD(this->get_hover_rot())));
}

void Striker::output(int x, int y, float r, float g, float b, void* font, char *string)
{
  glColor3f( r, g, b );
  glRasterPos2f(x, y);
  int len, i;
  len = (int)strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void Striker::display_power_bar(int inp,int endgame)
{
    if(endgame)
    {
        output(100.0f,40.0f,0.0f,0.0f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"Game Ended.");
        return;
    }

    if(!inp)
    {
        output(100.0f,40.0f,0.0f,0.0f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"Choose Coin Color: ");
        output(100.0f,32.0f,0.0f,0.0f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"1. Black");
        output(100.0f,24.0f,0.0f,0.0f,0.0f,GLUT_BITMAP_TIMES_ROMAN_24,"2. White");
        return;
    }

    glColor3f(0.0f,0.0f,0.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glBegin(GL_QUADS);
    glVertex2f(100.0f,-40.0f);
    glVertex2f(100.0f,40.0f);
    glVertex2f(120.0f,40.0f);
    glVertex2f(120.0f,-40.0f);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glColor3f(1.0f,1.0f,0.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glBegin(GL_QUADS);
    glVertex2f(100.2f,-39.8f);
    glVertex2f(100.2f,this->power);
    glVertex2f(119.8f,this->power);
    glVertex2f(119.8f,-39.8f);
    glEnd();
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}


float Striker::get_hover_x()
{
    return this->hover_x;
}

void Striker::set_hover_x(float x)
{
    this->hover_x = x;
}

float Striker::get_hover_y()
{
    return this->hover_y;
}

void Striker::set_hover_y(float x)
{
    this->hover_y = x;
}

float Striker::get_hover_rot()
{
    return this->hover_rot;
}

void Striker::set_hover_rot(float x)
{
    this->hover_rot = x;
}

float Striker::get_power()
{
    return this->power;
}

void Striker::set_power(float pow)
{
    this->power = pow;
}
