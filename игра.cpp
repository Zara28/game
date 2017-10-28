#include "TXLib.h"
#include <iostream>
#include <conio.h>
#include <time.h>

#include <fstream>
#include <string>

int MIN_COUNT_OF_GHOSTS = 2;
int MAX_COUNT_OF_GHOSTS = 3;
int HEROLIVES = 1;
int MAX_VX = 15;
int MIN_VX = 5;

int MAX_VY = 15;
int MIN_VY = 5;

string KOD;

struct BUTTON
{
    const char* textbutton;
    int minX;
    int maxX;
    void (*draw)();
    void (*onClick)(HDC* fon);
};

struct Hero
{
    int x;
    int y;
    int lives;
    HDC* photo;
};

struct Ghost
{
    int x;
    int y;
    int vx;
    int vy;
    int count_collisions;
    HDC* ghostPhoto;
};


void drawHelpString(int x, int y, const char* helpText);
void drowchar (int* x, int* y, HDC* ghostPhoto);
void drowboss (int* x, int* y, HDC* bossPhoto);
void drawhero (int* heroX, int* heroY, HDC* heroPhoto);
void dvizhenieVnutriKorobki ();
void management (int* heroX, int* heroY, int keyUp, int keyDown, int keyLeft, int keyRight);
void acceleration (Ghost *ghost);
void ikran (Ghost *ghost);
void drawTime ( int time1);
void drawLevel(int* time1, int count_of_ghosts, HDC *fon, HDC *fon4, Ghost *ghost_list, Ghost *ghostBoss, Hero* hero, bool hasBoss);
int stolknovenie (int* heroX, int* heroY, Ghost *ghost, HDC* heroPhoto, int *heroLives, int tjagestTravmy);
void pausa (HDC *cartinkaPausa);
void function1 ();
void function2 ();
void function3 ();

using namespace std;

void parametry()
{
     ifstream fout("Настройки игры.txt");
    while (fout) {
        string get;
        getline (fout, get);
        if (get.substr(0, strlen("MIN_COUNT_OF_GHOSTS = ")) == "MIN_COUNT_OF_GHOSTS = ") {
            MIN_COUNT_OF_GHOSTS = atoi(get.substr(strlen("MIN_COUNT_OF_GHOSTS = "), 100).c_str());
        }
        if (get.substr(0, strlen("MAX_COUNT_OF_GHOSTS = ")) == "MAX_COUNT_OF_GHOSTS = ") {
            MAX_COUNT_OF_GHOSTS = atoi(get.substr(strlen("MAX_COUNT_OF_GHOSTS = "), 100).c_str());
        }
        if (get.substr(0, strlen("MIN_VX = ")) == "MIN_VX = ") {
            MIN_VX = atoi(get.substr(strlen("MIN_VX = "), 100).c_str());
        }
        if (get.substr(0, strlen("MAX_VX = ")) == "MAX_VX = ") {
            MAX_VX = atoi(get.substr(strlen("MAX_VX = "), 100).c_str());
        }
        if (get.substr(0, strlen("MIN_VY = ")) == "MIN_VY = ") {
            MIN_VY = atoi(get.substr(strlen("MIN_VY = "), 100).c_str());
        }
        if (get.substr(0, strlen("MAX_VY = ")) == "MAX_VY = ") {
            MAX_VY = atoi(get.substr(strlen("MAX_VY = "), 100).c_str());
        }
        if (get.substr(0, strlen("HEROLIVES = ")) == "HEROLIVES = ") {
            HEROLIVES = atoi(get.substr(strlen("HEROLIVES = "), 100).c_str());
        }
         if (get.substr(0, strlen("KOD = ")) == "KOD = ") {
            KOD = get.substr(strlen("KOD = "), 100);
        }
    }
    fout.close(); // закрываем файл*/

    int dlina = strlen(KOD.c_str());

    MAX_COUNT_OF_GHOSTS = MAX_COUNT_OF_GHOSTS - dlina;
    MIN_COUNT_OF_GHOSTS = MIN_COUNT_OF_GHOSTS - dlina;
    HEROLIVES = HEROLIVES - dlina;
    MAX_VX = MAX_VX - dlina;
    MIN_VX = MIN_VX - dlina;

    MAX_VY = MAX_VY - dlina;
    MIN_VY = MIN_VY - dlina;
}

int main()
{
    bool game_over = false;
    parametry();
    txCreateWindow ( 500, 650);
    txSetFillColor(TX_WHITE);
    txClear();
    txSetColor(TX_WHITE);

    BUTTON buttons[3];
    buttons[0] = {"sdfsdf",    200, 250, function1};
    buttons[1] = {"sdfs789df", 300, 450, function2};
    buttons[2] = {"sdfs789df", 350, 500, function3};



    HDC cartinkaPausa = txLoadImage ("Pics/лес.bmp");
    HDC zastavka = txLoadImage ("Pics/заставка.bmp");
    while(!GetAsyncKeyState(VK_ESCAPE) && !game_over) {

        txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), zastavka, 0, 0);
        buttons[0].draw();
        buttons[1].draw();

        if ((txMouseButtons() & 1) && (txMouseY() > buttons[0].minX  && txMouseY() < buttons[0].maxX))
        {
            txSetColor(TX_WHITE);
            dvizhenieVnutriKorobki();
            game_over = true;
        }

        if ((txMouseButtons() & 1) && (txMouseY() > buttons[1].minX  && txMouseY() < buttons[1].maxX))
        {

            pausa (&cartinkaPausa);
        }

        txSleep(100);
    }
    txDeleteDC (&cartinkaPausa);
    txDeleteDC (&zastavka);


    return 0;
}



void drawhero (int* heroX, int* heroY, HDC* heroPhoto)
{
    txTransparentBlt (txDC(), *heroX - 49, *heroY - 70, 98, 137, *heroPhoto, 0, 0, TX_BLACK);
}

void drowchar (int* x, int* y, HDC* ghostPhoto)
{
    txTransparentBlt (txDC(), *x, *y, 153, 123, *ghostPhoto, 0, 0, TX_BLACK);
}

void drowboss (int* x, int* y, HDC* bossPhoto)
{
    txBitBlt (txDC(), *x, *y, 100, 200, *bossPhoto, 0, 0);
}


void function1 ()
{
    txSelectFont ("Mistral", 40, 10, false, false, false, false, 0);
    txSetColor (RGB (0, 81, 0));
    txRectangle (200, 200, 350, 290);
    txTextOut (200, 220,  "Начать игру" );
}

void function2 ()
{
    txSelectFont ("Mistral", 40, 10, false, false, false, false, 0);
    txSetColor (RGB (0, 81, 0));
    txRectangle (200, 300, 350, 400);
    txTextOut (200, 300,  "Правила" );
}
void function3 ()
{
    txSetColor (RGB (0, 81, 0));
    txRectangle (315, 460, 470, 550);
    txTextOut (325, 470,  "Начать игру" );
}

void acceleration (Ghost *ghost)
{

    ghost->x =  ghost->x +  ghost->vx;
    ghost->y =  ghost->y +  ghost->vy;

    if (ghost->vx > MAX_VX) {
        ghost->vx = MAX_VX;
    }
    if (ghost->vx < -MAX_VX) {
        ghost->vx = -MAX_VX;
    }
    if (ghost->vy > MAX_VY) {
        ghost->vy = MAX_VY;
    }
    if (ghost->vy < -MAX_VY) {
        ghost->vy = -MAX_VY;
    }

    ikran ( ghost);
}

void ikran (Ghost *ghost)
{
    if ( ghost->x > txGetExtentX()) {
       ghost->vx = - ghost->vx;
    }
    if ( ghost->y > txGetExtentY()) {
        ghost->vy = - ghost->vy;
    }
    if ( ghost->x < 0) {
        ghost->vx = - ghost->vx;
    }
    if ( ghost->y < 0) {
        ghost->vy = - ghost->vy;
    }


}

int stolknovenie (int* heroX, int* heroY, Ghost *ghost, HDC *heroPhoto, int *heroLives, int tjagestTravmy)
{

    COLORREF color = TX_BLACK;
    drawhero (heroX, heroY, heroPhoto);
    if ((*heroX - ghost->x < 30) && (*heroX - ghost->x > -30) &&
        (*heroY - ghost->y < 30) && (*heroY - ghost->y > -30)) {
        drawhero (heroX, heroY, heroPhoto);
        ghost->count_collisions = ghost->count_collisions + 1;
        *heroLives = *heroLives - tjagestTravmy;

         if (color == RGB (128, 0, 64)) {
           *heroLives = *heroLives - 1;
        }
        txSleep (5);

        return 1;
    }

    return 0;
}

void management (int* heroX, int* heroY, int keyUp, int keyDown, int keyLeft, int keyRight)
{
    if (GetAsyncKeyState (keyUp)) {
        *heroY = *heroY - 5;
    }
    if (GetAsyncKeyState (keyDown)) {
        *heroY = *heroY + 5;
    }
    if (GetAsyncKeyState (keyLeft)) {
        *heroX = *heroX - 5;
    }
    if (GetAsyncKeyState (keyRight)) {
        *heroX = *heroX + 5;
    }
}

void drawTime (int time1)
{

    txSetColor (TX_WHITE);
    time_t t;
    struct tm *t_m;
    t=time(NULL);
    t_m=localtime(&t);

    char vremya[100];
    sprintf (vremya, "Время = %d:%d:%d", t_m->tm_hour, t_m->tm_min, t_m->tm_sec);
    txSelectFont ("Comic Sans MS", 40, 10, false, false, false, false, 5);
    txTextOut (10, 10, vremya);
}

void pausa (HDC *cartinkaPausa)
{
        txSleep(100);
        txSetColor (TX_WHITE);
        BUTTON buttons[3];
        buttons[0] = {"sdfsdf",    200, 250, function1};
        buttons[1] = {"sdfs789df", 300, 400, function2};
        buttons[2] = {"FGJFJJF",   350, 500, function3};

        txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), *cartinkaPausa, 0, 0);

        drawHelpString(10, 10, "Управление стрелками");
        drawHelpString(10, 50, "Берегись привидений!");
        drawHelpString(10, 90, "Не врезайся в припятствия!");
        drawHelpString(10, 130, "Чтобы продолжить игру нажмите Р");
        drawHelpString(10, 170, "Чтобы вернуться в меню (если еще ");
        drawHelpString(10, 210, "не играли) нажмите Р");
        drawHelpString(10, 250, "Чтобы начать игру с начала  нажмите ");
        drawHelpString(10, 290, " кнопку Начать игру");


        buttons[2].draw();

        while(!GetAsyncKeyState('P')) {
            if (txMouseButtons() & 1 &&
                txMouseX() >=buttons[2].minX &&
                txMouseX() <=buttons[2].maxX &&
                txMouseY() >=buttons[2].minX &&
                txMouseY() <=buttons[2].maxX) {
                txSetColor(TX_WHITE);
                dvizhenieVnutriKorobki();
            }
            txSleep(10);
        }

        txSleep(100);
}


void gameOver (HDC *fonGameOver, int count_collisions, int count_collisions_with_boss, int heroLives, int time1)
{
    txSetColor(TX_WHITE);
    txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), *fonGameOver, 0, 0);

    char collisions[100];
    sprintf(collisions, "count_collisions = %d", count_collisions);
    drawHelpString(10, 10, collisions);

    char collisions_with_boss[100];
    sprintf(collisions_with_boss, "count_collisions_with_boss = %d", count_collisions_with_boss);
    drawHelpString(10, 40, collisions_with_boss);

    char hero[100];
    sprintf (hero, "heroLives = %d", heroLives);
    drawHelpString(10, 70, hero);
}

void drawHelpString(int x, int y, const char* helpText)
{
    char help[350];
    sprintf(help, helpText);
    txSelectFont ("Mistral", 40, 10, false, false, false, false, 0);
    txTextOut(x, y, help);
}

void drawLevel(int* time1, int count_of_ghosts, HDC *fonLevel1, HDC *fon4pausa, Ghost *ghost_list, Ghost *ghostBoss, Hero* hero, bool hasBoss)
{
    COLORREF color = TX_BLACK;

    if (count_of_ghosts > MAX_COUNT_OF_GHOSTS) {
        count_of_ghosts = MAX_COUNT_OF_GHOSTS;
    }
    if (count_of_ghosts < MIN_COUNT_OF_GHOSTS) {
        count_of_ghosts = MIN_COUNT_OF_GHOSTS;
    }

    if ((hero->lives > HEROLIVES)) {
        hero->lives = HEROLIVES;
    }

    hero->x = 1;
    hero->y = 1;
    while (color != RGB(0, 0, 255) && hero->lives > 0) {
        txBitBlt (txDC(), 0, 0, txGetExtentX(), txGetExtentY(), *fonLevel1, 0, 0);

        management  (&hero->x, &hero->y, VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);

        *time1 = *time1 + 1;
        drawTime (*time1);

        for (int i = 0; i < count_of_ghosts; i++) {
            acceleration (&ghost_list[i]);
            stolknovenie (&hero->x,  &hero->y,  &ghost_list[i], hero->photo, &hero->lives, 1);
            drowchar ( &ghost_list[i].x,  &ghost_list[i].y,  ghost_list[i].ghostPhoto);
        }

        if (hasBoss) {
            acceleration (ghostBoss);
            stolknovenie (&hero->x,  &hero->y,  ghostBoss, hero->photo, &hero->lives, 5);
            drowboss ( &ghostBoss->x,  &ghostBoss->y,  ghostBoss->ghostPhoto);
        }

        color = Win32::GetPixel(*fonLevel1, hero->x, hero->y);

        if (color == RGB (128, 0, 64)) {
            hero->lives = hero->lives - 1;
        }

        if (GetAsyncKeyState('P')) {
            pausa (fon4pausa);
        }
        if (txMouseX() >= 0 && txMouseX () <= 600 && txMouseY () >= 550 && txMouseY () <= 650 )
                 {
                txSetColor(TX_WHITE);

                txSelectFont ("Mistral", 40, 10, false, false, false, false, 0);
                txDrawText ( 140, 120, 400, 300, "Ghbdbltybt");
            }

        txSleep (30);
    }
}

void dvizhenieVnutriKorobki ()
{

    HDC  cartinkaPausa = txLoadImage ("Pics/лес.bmp");
    HDC  fonGameOver = txLoadImage ("Pics/конец.bmp");
    HDC  fonLevel1 = txLoadImage ("Pics/замок новый.bmp");
    HDC  fonLevel2 = txLoadImage ("Pics/library.bmp");
    HDC  fonLevel3 = txLoadImage ("Pics/stairway.bmp");
    HDC  fonLevel4 = txLoadImage ("Pics/bosslevel.bmp");
    HDC  ghostPhoto = txLoadImage ("Pics/ghost.bmp");
    HDC  bossPhoto = txLoadImage ("Pics/босс.bmp");
    HDC  heroPhoto = txLoadImage ("Pics/человек.bmp");


    Hero hero = {50, 50, HEROLIVES, &heroPhoto};

    Ghost ghost_list[MAX_COUNT_OF_GHOSTS];
    for (int i = 0; i < MAX_COUNT_OF_GHOSTS; i++) {
        ghost_list[i].x = rand() % txGetExtentX();
        ghost_list[i].y = rand() % txGetExtentY();
        ghost_list[i].vx = MIN_VX + rand() % (MAX_VX - MIN_VX + 1);//Деление с остатком. ВОзвращает число от 0 до 12-1
        ghost_list[i].vy = MIN_VY + rand() % (MAX_VY - MIN_VY + 1);
        ghost_list[i].count_collisions = 0;
        ghost_list[i].ghostPhoto = &ghostPhoto;
    }

    Ghost ghostBoss = { 200, 50,  10,   9,   0,              &bossPhoto};


    int time1 = 0;
    int count_collisions = 0;

    drawLevel(&time1, 5,  &fonLevel1, &cartinkaPausa, ghost_list, &ghostBoss, &hero, false);
    drawLevel(&time1, 10, &fonLevel2, &cartinkaPausa, ghost_list, &ghostBoss, &hero, false);
    drawLevel(&time1, 15, &fonLevel3, &cartinkaPausa, ghost_list, &ghostBoss, &hero, false);
    drawLevel(&time1, 0,  &fonLevel4, &cartinkaPausa, ghost_list, &ghostBoss, &hero, true);

    count_collisions = 0;
    for (int i = 0; i < MAX_COUNT_OF_GHOSTS; i++) {
        count_collisions = count_collisions + ghost_list[i].count_collisions;
    }
    gameOver (&fonGameOver, count_collisions, ghostBoss.count_collisions, hero.lives, time1);
    txSleep (100);

    txDeleteDC (fonLevel1);
    txDeleteDC (fonLevel2);
    txDeleteDC (fonLevel3);
    txDeleteDC (fonLevel4);
    txDeleteDC (fonGameOver);
    txDeleteDC (cartinkaPausa);
    txDeleteDC (ghostPhoto);
    txDeleteDC (heroPhoto);
    txDeleteDC (bossPhoto);
}
