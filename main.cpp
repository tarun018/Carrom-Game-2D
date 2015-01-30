#include <iostream>
#include <cmath>
#include <string>
#include <stdlib.h>
#include <GL/glut.h>
#include<glut.h>
#include<string.h>
#include <sys/time.h>
#include <time.h>
#include "Coin.h"
#include "Player.h"
#include "Board.h"
#include "Striker.h"
using namespace std;

//Macro Definitions
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
#define RAD2DEG(rad) (rad * 180 / PI)
#define ROTATEX(x,y,deg) ((x * cos(DEG2RAD(deg)))-(y * sin(DEG2RAD(deg))))
#define ROTATEY(x,y,deg) ((x * sin(DEG2RAD(deg)))+(y * cos(DEG2RAD(deg))))
#define RED_COIN coin[8]
#define NUMBER_COINS 9

// Function Declarations
void drawScene();
void update(int value);
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void handleMouseclick(int button, int state, int x, int y);
void handleMotionClick(int x, int y);
void output(int x, int y, float r, float g, float b, void* font, char *string);
void reset_new_chance();

//GLobal Variables
int first_display = 0;
int display_hover = 1;
int endgame = 0;
float windowWidth,windowHeight;
int play = 0;
int processing[NUMBER_COINS+1][NUMBER_COINS+1]={{0}};
time_t curtime,prevtime;
int input = 0,chosencolor = 0;
struct timeval current;


//Object Declarations
Board board;
Coin coin[NUMBER_COINS];
Striker striker;
Player player1("Tarun");



int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    windowWidth = w * 2 / 3;
    windowHeight = h * 2 / 3;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("Carrom");
    initRendering();

    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutMouseFunc(handleMouseclick);
    glutMotionFunc(handleMotionClick);
//    glutMouseWheelFunc(mouseWheel);
    glutReshapeFunc(handleResize);
    glutTimerFunc(10, update, 0);

    glutMainLoop();
    return 0;
}

void reset_new_chance()
{
    display_hover = 1;
    striker.reset_Striker();
    play = 0;
}

void initialize_board()
{
    float rtang = 0.0f;
    for(int i = 0; i < NUMBER_COINS/2 ; i++)
    {
        coin[i] = Coin(ROTATEX(10.0f,0.0f,rtang),ROTATEY(10.0f,0.0f,rtang),3.5f,0.0f,0.0f,0.0f,5.0f,10);
        coin[i].createCoin();
        rtang += 90.0f;
    }

    rtang = 45.0f;
    for(int i = NUMBER_COINS/2; i < NUMBER_COINS ; i++)
    {
        coin[i] = Coin(ROTATEX(10.0f,0.0f,rtang),ROTATEY(10.0f,0.0f,rtang),3.5f,1.0f,1.0f,1.0f,5.0f,10);
        coin[i].createCoin();
        rtang += 90.0f;
    }

    RED_COIN = Coin(0.0f,0.0f,3.5f,1.0f,0.0f,0.0f,5.0f,50);
    striker = Striker(0.0f,-55.5f,4.5f,0.0f,1.0f,0.0f,8.0f,-5);
    RED_COIN.createCoin();
    striker.createCoin();
}


void drawScene() {

    if(!first_display)
    {
        gettimeofday(&current,NULL);
        curtime = current.tv_sec;
        prevtime = curtime;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();

    if(input)
    {
        int start = (chosencolor-1)*(NUMBER_COINS/2);
        int en = (start + NUMBER_COINS/2);
        for(int i=start; i<en && RED_COIN.isPocketed() ;i++)
        {
            if(!coin[i].isPocketed())
                break;
            else if(i==en-1)
                endgame = 1;
        }
    }

    glTranslatef(0.0f, 0.0f, -200.0f);

    board.createBoard();
    if(!(first_display++))
        initialize_board();


    if(play==2)
        reset_new_chance();

    for(int i = 0; i < NUMBER_COINS ; i++)
        coin[i].createCoin();

    if(!striker.isPocketed())
        striker.createCoin();

    if(display_hover)
    {
        glColor3f(0.0f,0.0f,1.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(striker.get_x(),striker.get_y());
        glVertex2f(striker.get_hover_x(),striker.get_hover_y());
        glEnd();
    }

    striker.display_power_bar(input,endgame);
    board.printScore(player1);

    glPopMatrix();
    glutSwapBuffers();
}

void detect_collision()
{
    int collided[NUMBER_COINS+1][NUMBER_COINS+1]={{0}};
    for(int i=0;i<NUMBER_COINS;i++)
    {
        for(int j=0;j<NUMBER_COINS;j++)
        {
            if(!collided[i][j] && coin[i].isCollided(coin[j]) && i!=j && processing[i][j]==0)
            {
                collided[i][j] = 1;
                collided[j][i] = 1;
                coin[i].collide(&coin[j]);
                processing[i][j]=1;
                processing[j][i]=1;
                i = -1;
                break;
            }
            else if(!coin[i].isCollided(coin[j]) && i!=j && processing[i][j]==1)
            {
                processing[i][j]=0;
                processing[j][i]=0;
            }
        }
        if(!collided[NUMBER_COINS][i] && striker.isCollided(coin[i]) && processing[NUMBER_COINS][i]==0)
        {
            collided[NUMBER_COINS][i] = 1;
            collided[i][NUMBER_COINS] = 1;
            striker.collide(&coin[i]);
            processing[i][NUMBER_COINS]=1;
            processing[NUMBER_COINS][i]=1;
            i = -1;
            break;
        }
        else if(!striker.isCollided(coin[i]) && processing[9][i]==1)
        {
            processing[i][9]=0;
            processing[9][i]=0;
        }
    }
}

void update(int value)
{
    gettimeofday(&current,NULL);
    curtime = current.tv_sec;

    if(curtime-prevtime == 1 && play != 1 && input && !endgame)
        player1.setScore(player1.getScore() - 1);

    if(play == 1)
    {
        for(int i=0;i<NUMBER_COINS;i++)
            coin[i].friction_impact();
        striker.friction_impact();

        detect_collision();

        for(int i=0;i<NUMBER_COINS;i++)
        {
            if(board.checkPocketed(&coin[i]))
            {
                coin[i].pocketCoin();
                coin[i].docketCoin();
                player1.setScore(player1.getScore() + coin[i].get_points());
            }
            else if(!coin[i].isPocketed())
            {
                board.checkWallCollision(&coin[i]);
                coin[i].set_x((coin[i].get_vx()*1)+coin[i].get_x());
                coin[i].set_y((coin[i].get_vy()*1)+coin[i].get_y());
            }
        }

        if(board.checkPocketed(&striker))
        {
            player1.setScore(player1.getScore() + striker.get_points());
            striker.set_vx(0.0f);
            striker.set_vy(0.0f);
            striker.pocketCoin();
            striker.docketCoin();
        }
        else if(!striker.isPocketed())
        {
            board.checkWallCollision(&striker);
            striker.set_x((striker.get_vx()*1)+striker.get_x());
            striker.set_y((striker.get_vy()*1)+striker.get_y());
        }


        for(int i=0; i<9 && striker.get_vx()==0.0f && striker.get_vy()==0.0f; i++)
        {
            if(coin[i].get_vx()!=0.0f || coin[i].get_vy()!=0.0f)
                break;
            else if(i==8)
                play = 2;
        }

    }

    prevtime = curtime;
    glutTimerFunc(10, update, 0);
}



// Initializing some openGL 3D rendering options
void initRendering() {

    //glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(1.0f, 1.0f, 0.7f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }

    if(key==32 && play==0 && input && endgame != 1)
    {

        display_hover = 0;
        striker.Striker_shoot();
        play = 1;
    }
    if (key == 67 || key == 99)
    {
        if(!play)
            striker.hover_move(1);
    }
    if (key == 65 || key == 97)
    {
        if(!play)
            striker.hover_move(-1);
    }

    if(key=='1' && !input++)
    {
        chosencolor = 1;
        for(int i=4;i<8;i++)
            coin[i].set_points(-5);
    }

    if(key=='2' && !input++)
    {
        chosencolor = 2;
        for(int i=0;i<4;i++)
            coin[i].set_points(-5);
    }

}

void handleKeypress2(int key, int x, int y) {

    if(key == GLUT_KEY_LEFT)
    {
        if(!play)
        {
            striker.Striker_move(-1);
            striker.Striker_move(-1);
        }
    }
    if(key == GLUT_KEY_RIGHT)
    {
        if(!play)
        {
            striker.Striker_move(1);
            striker.Striker_move(1);
        }
    }
    if (key == GLUT_KEY_UP)
    {
        if(striker.get_power() < 35 && !play)
        {
            striker.set_power(striker.get_power()+5.0f);
        }
    }
    if (key == GLUT_KEY_DOWN)
    {
        if(striker.get_power() > -35 && !play)
        {
            striker.set_power(striker.get_power()-5.0f);
        }
    }
}

int isRightClicked = 0;
int isLeftClicked = 0;
int drag_on_left = 0;
int drag_on_right = 0;
int just_closed = 0;
int position[2];

void handleMouseclick(int button, int state, int x, int y) {

    drag_on_right = 0;
    if (button == GLUT_RIGHT_BUTTON)
        isRightClicked = (!state);
    if (button == GLUT_LEFT_BUTTON)
        isLeftClicked = (!state);

    if (state==1 && button == GLUT_LEFT_BUTTON)
    {
        if(play==0 && input && endgame != 1)
        {
            display_hover = 0;
            striker.Striker_shoot();
            play = 1;
        }
    }

    if(isRightClicked)
        drag_on_right = 1 ;

    if(isLeftClicked)
        drag_on_left = 1;

}
int position_drag[2]; //to check left or right
int position_drag_initial[2]; // intensity
int dir=0;
void handleMotionClick(int x,int y)
{
    if(drag_on_right)
    {
        if(position_drag[0]>x)
        {
            if(dir!=-1)
            {
                position_drag_initial[0]=x;
            }
            dir=-1;
            for(int i=-1;i<(position_drag_initial[0]-x)/100;i++)
            {
                if(!play)
                    striker.Striker_move(-1);
            }
        }
        if(position_drag[0]<x)
        {
            if(dir!=1)
            {
                position_drag_initial[0]=x;
            }
            dir=1;
            for(int i=-1;i<(x-position_drag_initial[0])/100;i++)
            {
                if(!play)
                    striker.Striker_move(1);
            }
        }
        position_drag[0]=x;
    }
    if(drag_on_left)
    {
        if(position_drag[0]>x)
        {
            if(dir!=-1)
            {
                position_drag_initial[0]=x;
            }
            dir=-1;
            for(int i=-1;i<(position_drag_initial[0]-x)/10000;i++)
            {
                if(!play)
                    striker.hover_move(-1);
            }
        }
        if(position_drag[0]<x)
        {
            if(dir!=1)
            {
                position_drag_initial[0]=x;
            }
            dir=1;
            for(int i=-1;i<(x-position_drag_initial[0])/10000;i++)
            {
                if(!play)
                    striker.hover_move(1);
            }
        }
        position_drag[0]=x;
    }
}
