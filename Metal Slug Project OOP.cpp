#include <iostream>
#include <time.h>
#include <raylib.h>
using namespace std;
#define ScreenHeight 600
#define ScreenWidth 1000
#define MAX_BULLETS 5

struct Player
{
    int x[2], y[2];
    bool isAlive;
    float PHealth;
    Rectangle PCollider;
    Texture2D PImage[10];
    int PHeight[2], PWidth[2];
};

struct Bullet
{
    int x, y;
    Texture2D BImage;
    bool active;
    int BHeight, BWidth;
    Rectangle BCollider;
};

struct Enemy
{
    int x, y;
    int Espeed;
    Rectangle ECollider;
    float EHealth;
    Bullet bullets;
    Texture2D EImage;
    void (*fptr)(Enemy);
    int EHeight, EWidth;
};

struct BG
{
    int ri, ci;
};

void LoadPlayerImages(Player& P)
{
    P.PImage[0] = LoadTexture("PlayerBody.png");
    P.PHeight[0] = 150, P.PWidth[0] = 100;
    P.PImage[1] = LoadTexture("Leg1.png");
    P.PImage[2] = LoadTexture("Leg2.png");
    P.PImage[3] = LoadTexture("Leg3.png");
    P.PImage[4] = LoadTexture("Leg4.png");
    P.PImage[5] = LoadTexture("Leg5.png");
    P.PHeight[1] = 80, P.PWidth[1] = 60;

    P.x[0] = P.PWidth[0] -5, P.y[0] = ScreenHeight - P.PHeight[0]+25;
    P.x[1] = P.PWidth[1] + 20, P.y[1] = ScreenHeight - P.PHeight[1] - 20;
}

void RespawnEnemy(Enemy& E)
{
    E.x = ScreenWidth+ rand() % 500+100;
    E.y = ScreenHeight - E.EHeight + 25;
}

void RespawnHelicopter(Enemy& E)
{
    E.x = ScreenWidth + rand() % 500 + 100;
    E.y = E.EHeight -20;
}

//void HealthBar(Player& P)
//{
//    DrawRectangle(20, 20, 200, 20, RED);
//    DrawRectangle(20, 20, (int)(200 * (P.Health / 100.0f)), 20, GREEN);
//}


// **********************MAINS************
int main()
{
    Player P;
    Bullet bullets[MAX_BULLETS] = {};
    Enemy E,EnemyMan,Helicopter;  //Can be array
    P.isAlive = true;
    P.PHealth = 100.0;
    BG b1, b2;
    Texture2D bg1, bg2,Title;
    bool GameStart = false;
    int score = 0;

    InitWindow(ScreenWidth, ScreenHeight, "METAL SLUG");
    SetTargetFPS(60);

    E.EImage = LoadTexture("Enemy.png");
    E.EHeight = 150, E.EWidth = 100;
    EnemyMan.EImage = LoadTexture("Enemy_Man.png");
    EnemyMan.EHeight = 150, EnemyMan.EWidth = 100;
    Helicopter.EImage = LoadTexture("Heli.png");
    Helicopter.EHeight = 150, Helicopter.EWidth = 100;

    for(int i=0;i<MAX_BULLETS;i++)
    {
        bullets[i].BImage = LoadTexture("Bullet.png");
        bullets[i].BHeight = 10, bullets[i].BWidth = 20;
        bullets[i].x = P.PWidth[0] + 60, bullets[i].y = ScreenHeight - P.PHeight[0] + 40;
        bullets[i].active = false;
    }


    LoadPlayerImages(P);

    bg1 = LoadTexture("Back1.png");
    bg1.height = ScreenHeight, bg1.width = ScreenWidth;
    bg2 = LoadTexture("Back2.png");
    bg2.height = ScreenHeight, bg2.width = ScreenWidth;
    Title = LoadTexture("Title.png");

    E.x = ScreenWidth,E.y = ScreenHeight - E.EHeight+25 ;
    EnemyMan.x = ScreenWidth, EnemyMan.y = ScreenHeight - EnemyMan.EHeight+25 ;
    Helicopter.x = ScreenWidth, Helicopter.y =  Helicopter.EHeight -20;

    int TotalPlayerHeight = P.PHeight[0] + P.PHeight[1];
    int TotalPlayerWidth = P.PWidth[0];

    b1.ri = 0, b1.ci = 0;
    b2.ri = 0, b2.ci = ScreenWidth;

    int FrameCount = 0;
    int LegFrame = 1;

    float PSpeed = 0;
    bool Jump = false;

    for (int i = 1; i <= 100; i++)  // Title Show
    {
        Title.height = 4*i, Title.width = 6*i;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Title, ScreenWidth / 2 - Title.width / 2, 20, WHITE);
        EndDrawing();
    }

    while (!WindowShouldClose() && !GameStart) //Start Menu
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Title,ScreenWidth/2-Title.width/2, 20, WHITE);
        DrawText("PRESS ENTER TO START", ScreenWidth / 2 - 180, ScreenHeight / 2 + 150, 30, WHITE);
        if (IsKeyPressed(KEY_ENTER))
        {
            GameStart = true;
        }
        EndDrawing();
    }

    while (!WindowShouldClose())
    {
        
        
        BeginDrawing();
        DrawTexture(bg1, b1.ci, b1.ri, WHITE);
        DrawTexture(bg2, b2.ci, b2.ri, WHITE);


        //COLLIDER CHECKERS

        //DrawRectangle(P.x[0], P.y[0], TotalPlayerWidth-30, TotalPlayerHeight-130, WHITE);  //Player collider checker
        //DrawRectangle(E.x+15,E.y, E.EWidth-30,E.EHeight, WHITE); //Enemy Collider Checker
        //DrawRectangle(EnemyMan.x, EnemyMan.y, EnemyMan.EWidth-10, EnemyMan.EHeight, WHITE);  //EnemyMan Collider Checker
        //DrawRectangle(Helicopter.x+16, Helicopter.y, Helicopter.EWidth+30, Helicopter.EHeight-80, WHITE); //Helicopter Collider Checker


        DrawTexture(P.PImage[LegFrame], P.x[1], P.y[1], WHITE);
        DrawTexture(P.PImage[0], P.x[0], P.y[0], WHITE);

        DrawTexture(E.EImage, E.x, E.y, WHITE);
        DrawTexture(EnemyMan.EImage, EnemyMan.x, EnemyMan.y, WHITE);
        DrawTexture(Helicopter.EImage, Helicopter.x, Helicopter.y, WHITE);


        E.x --;
        EnemyMan.x--;
        Helicopter.x --;
        if (IsKeyDown(KEY_D))
        {
            FrameCount++;

            if (FrameCount % 10 == 0)
            {
                LegFrame++;
                if (LegFrame > 5)
                {
                    LegFrame = 1;
                }
            }
            b1.ci -= 3;
            b2.ci -= 3;
            E.x -= 3;
            EnemyMan.x -= 3;
            Helicopter.x -= 3;
        }
        else
        {
            FrameCount = 0;
            LegFrame = 1;
        }
        if (IsKeyPressed(KEY_W) && !Jump)
        {
            PSpeed = -15;
            Jump = true;
        }
        P.y[0] += PSpeed;
        P.y[1] += PSpeed;
        PSpeed += 0.5;

        if (P.y[0] >= ScreenHeight - P.PHeight[0] + 25)
        {
            P.y[0] = ScreenHeight - P.PHeight[0] + 25;
            P.y[1] = ScreenHeight - P.PHeight[1] - 25;
            PSpeed = 0;
            Jump = false;
        }

        Rectangle PlayerR = { (float)P.x[0],(float)P.y[0],(float)TotalPlayerWidth-30,(float)TotalPlayerHeight-130};
        Rectangle EnemyR = { (float)E.x+15,(float)E.y,(float)E.EWidth-30,(float)E.EHeight};
        Rectangle EnemyManR = { (float)EnemyMan.x,(float)EnemyMan.y,(float)EnemyMan.EWidth-10,(float)EnemyMan.EHeight };
        Rectangle HelicopterR = { (float)Helicopter.x+16,(float)Helicopter.y,(float)Helicopter.EWidth+30,(float)Helicopter.EHeight-80 };

        if (IsKeyPressed(KEY_SPACE)) 
        {
            for (int i = 0; i < MAX_BULLETS; i++) 
            {
                if (bullets[i].active==false) 
                {
                    bullets[i].active = true;
                    bullets[i].x = P.x[0] + P.PWidth[0] - 30;
                    bullets[i].y = P.y[0] + 20;
                    break;
                }
            }
        }

        for (int i = 0; i < MAX_BULLETS; i++) 
        {
            if (bullets[i].active) 
            {
                bullets[i].x += 10;
                if (bullets[i].x > ScreenWidth) 
                {
                    bullets[i].active = false;
                }
                bullets[i].BCollider = { (float)bullets[i].x, (float)bullets[i].y,(float)bullets[i].BWidth, (float)bullets[i].BHeight };
                if (CheckCollisionRecs(bullets[i].BCollider, EnemyR))
                {
                    score++;
                    bullets[i].active = false;
                    RespawnEnemy(E);
                }
                if ( CheckCollisionRecs(bullets[i].BCollider, EnemyManR))
                {
                    score++;
                    bullets[i].active = false;
                    RespawnEnemy(EnemyMan);
                }
                if( CheckCollisionRecs(bullets[i].BCollider, HelicopterR))
                {
                    score++;
                    bullets[i].active = false;
                    RespawnEnemy(Helicopter);
                }
            }
        }
        for (int i = 0; i < MAX_BULLETS; i++)
        {
            if (bullets[i].active)
            {
                DrawTexture(bullets[i].BImage, bullets[i].x, bullets[i].y, WHITE);
            }
        }

        
        if (b1.ci <= -ScreenWidth)
        {
            b1.ci *= -1;
        }

        if (b2.ci <= -ScreenWidth)
        {
            b2.ci *= -1;
        }

        if (E.x <= 0)
        {
            RespawnEnemy(E);
        }
        if (EnemyMan.x <= 0)
        {
            RespawnEnemy(EnemyMan);
        }
        if (Helicopter.x <= 0)
        {
            RespawnHelicopter(Helicopter);
        }
        DrawText(TextFormat("SCORE: %d", score), ScreenWidth - 200, 15, 20, WHITE);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}
