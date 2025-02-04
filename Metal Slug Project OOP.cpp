#include <iostream>
#include <time.h>
#include <raylib.h>
using namespace std;
#define ScreenHeight 600
#define ScreenWidth 1000

struct Player
{
    int x,y;
    bool isAlive;
    float PHealth;
    Rectangle PCollider;
};
/// MY NAME IS ABDULAH THE LOVE OF HAMZA 
struct Bullet
{
    int x, y;
    Texture2D BImage;
    bool active;
    Rectangle Collider;
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
};

struct BG
{
    int ri, ci;
};


//void HealthBar(Player& P)
//{
//    DrawRectangle(20, 20, 200, 20, RED);
//    DrawRectangle(20, 20, (int)(200 * (P.Health / 100.0f)), 20, GREEN);
//}


// **********************MAINS************
int main()
{
    BG b1, b2;
    Texture2D bg1, bg2;
    
    InitWindow(ScreenWidth, ScreenHeight, "METAL SLUG");
    SetTargetFPS(60);

    bg1 = LoadTexture("Back1.png");
    bg1.height = ScreenHeight, bg1.width = ScreenWidth;
    bg2 = LoadTexture("Back2.png");
    bg2.height = ScreenHeight, bg2.width = ScreenWidth;


    b1.ri = 0, b1.ci = 0;
    b2.ri = 0, b2.ci = ScreenWidth;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        DrawTexture(bg1, b1.ci, b1.ri, WHITE);
        DrawTexture(bg2, b2.ci, b2.ri, WHITE);

        if (IsKeyDown(KEY_RIGHT))
        {
            b1.ci -= 3;
            b2.ci -= 3;
        }
        
        if (b1.ci <= -ScreenWidth)
        {
            b1.ci *= -1;
        }

        if (b2.ci <= -ScreenWidth)
        {
            b2.ci *= -1;
        }
        EndDrawing();

    }
    CloseWindow();
    return 0;
}