#include <iostream>
#include <time.h>
#include <raylib.h>
using namespace std;
#define ScreenHeight 600
#define ScreenWidth 1000
#define MAX_BULLETS 3

struct Player
{
    int x[3], y[3];
    bool isAlive;
    float PHealth;
    Rectangle PCollider;
    Texture2D PImage[10];
    int PHeight[2], PWidth[2];
};

struct Shield {
    float health = 100.0f;
    Texture2D SImage;
    bool active = false;
};

struct Bullet
{
    int x, y;
    Texture2D BImage;
    bool active;
    int BHeight, BWidth;
    Rectangle BCollider;
    int vel_x;
    int vel_y;
};

struct Enemy
{
    int x, y;
    int Espeed;
    Rectangle ECollider;
    float EHealth;
    Bullet bullets;
    Texture2D EImage;
    int EHeight, EWidth;
    int timer;
    int bulletFrame;
    int bulletsActive = 0;
    void (*loadImage)(Enemy&);
};

struct BG
{
    int ri, ci;
};


void LoadEnemyImage(Enemy& enemy) {
    enemy.EImage = LoadTexture("Enemy.png");
    enemy.EHeight = 150;
    enemy.EWidth = 100;
}

void LoadEnemyManImage(Enemy& enemy) {
    enemy.EImage = LoadTexture("Enemy_Man.png");
    enemy.EHeight = 150;
    enemy.EWidth = 100;
}

void LoadHelicopterImage(Enemy& enemy) {
    enemy.EImage = LoadTexture("Heli.png");
    enemy.EHeight = 150;
    enemy.EWidth = 100;
}


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

    P.PImage[6] = LoadTexture("Player_Shoot.png");

    P.x[0] = P.PWidth[0] - 5, P.y[0] = ScreenHeight - P.PHeight[0] + 25;
    P.x[1] = P.PWidth[1] + 20, P.y[1] = ScreenHeight - P.PHeight[1] - 20;
    P.x[2] = P.PWidth[0] - 5, P.y[2] = ScreenHeight - P.PHeight[0] - 15;
}

void RespawnEnemy(Enemy& E)
{
    E.x = ScreenWidth + rand() % 500 + 100;
    E.y = ScreenHeight - E.EHeight + 25;
}

void RespawnHelicopter(Enemy& E)
{
    E.x = ScreenWidth + rand() % 500 + 100;
    E.y = E.EHeight - 20;
}


void LoadPlayerBulletImages(Bullet bullets[], Player P, const char* Fname)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i].BImage = LoadTexture(Fname);
        bullets[i].BHeight = 10, bullets[i].BWidth = 20;
        bullets[i].x = P.PWidth[0] + 60, bullets[i].y = ScreenHeight - P.PHeight[0] + 40;
        bullets[i].active = false;
        bullets[i].vel_x = 0;
        bullets[i].vel_y = 0;
    }
}

void LoadEnemyBulletImages(Bullet enemyBullets[], const char* Fname)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        enemyBullets[i].BImage = LoadTexture(Fname);
        enemyBullets[i].BHeight = 10, enemyBullets[i].BWidth = 20;
        enemyBullets[i].active = false;
    }
}

void EnemyBulletShooting(Enemy& E, Bullet enemyBullets[MAX_BULLETS])
{
    if (E.x < ScreenWidth) {
        if (E.timer != 80) {
            E.timer++;
        }
        else {
            if (E.bulletsActive < MAX_BULLETS && E.bulletFrame >= (rand() % 81 + 10)) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!enemyBullets[i].active) {
                        enemyBullets[i].active = true;
                        enemyBullets[i].x = E.x;
                        enemyBullets[i].y = E.y;
                        E.bulletsActive++;
                        break;
                    }
                }
                E.bulletFrame = 0;
            }
            E.bulletFrame++;
            if (E.bulletsActive >= MAX_BULLETS) {
                E.timer = 0;
                E.bulletFrame = 0;
                E.bulletsActive = 0;
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (enemyBullets[i].active) {
            enemyBullets[i].x -= 10;
            if (enemyBullets[i].x < 0) {
                enemyBullets[i].active = false;
            }
            DrawTexture(enemyBullets[i].BImage, enemyBullets[i].x, enemyBullets[i].y, WHITE);
        }
    }
}

void HelicopterBulletShooting(Enemy& E, Bullet HeliBullets[MAX_BULLETS])
{
    if (E.x < ScreenWidth) {
        if (E.timer != 80) {
            E.timer++;
        }
        else {
            if (E.bulletsActive < MAX_BULLETS && E.bulletFrame >= (rand() % 51 + 50)) {
                for (int i = 0; i < MAX_BULLETS; i++) {
                    if (!HeliBullets[i].active) {
                        HeliBullets[i].active = true;
                        HeliBullets[i].x = E.x + 50;
                        HeliBullets[i].y = E.y + 50;
                        E.bulletsActive++;
                        break;
                    }
                }
                E.bulletFrame = 0;
            }
            E.bulletFrame++;
            if (E.bulletsActive >= MAX_BULLETS) {
                E.timer = 0;
                E.bulletFrame = 0;
                E.bulletsActive = 0;
            }
        }
    }

    for (int i = 0; i < MAX_BULLETS; i++) {
        if (HeliBullets[i].active) {
            HeliBullets[i].y += 10;
            if (HeliBullets[i].y > ScreenHeight - 50) {
                HeliBullets[i].active = false;
            }
            DrawTexture(HeliBullets[i].BImage, HeliBullets[i].x, HeliBullets[i].y, WHITE);
        }
    }
}

void ShieldHealth(Shield& S)
{
    if (S.health > 0) {
        DrawRectangle(20, 50, 200, 20, GRAY);
        DrawRectangle(20, 50, (int)(200 * (S.health / 100.0f)), 20, BLUE);
    }
}

void HealthBar(Player& P)
{
    DrawRectangle(20, 20, 200, 20, RED);
    DrawRectangle(20, 20, (int)(200 * (P.PHealth / 100.0f)), 20, GREEN);
}

void PlayerBulletAndEnemyCollisionCheck(Bullet bullets[], int& score, Enemy& E, Enemy& EnemyMan, Enemy& Helicopter, Rectangle EnemyR, Rectangle EnemyManR, Rectangle HelicopterR)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].active)
        {
            if (bullets[i].vel_y != 0)
            {
                bullets[i].x += bullets[i].vel_x;
                bullets[i].y += bullets[i].vel_y;
            }
            else
            {
                bullets[i].x += 10;
            }

            if (bullets[i].x < 0 || bullets[i].x > ScreenWidth ||
                bullets[i].y < 0 || bullets[i].y > ScreenHeight)
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
            if (CheckCollisionRecs(bullets[i].BCollider, EnemyManR))
            {
                score++;
                bullets[i].active = false;
                RespawnEnemy(EnemyMan);
            }
            if (CheckCollisionRecs(bullets[i].BCollider, HelicopterR))
            {
                score++;
                bullets[i].active = false;
                RespawnHelicopter(Helicopter);
            }
        }
    }
}

void HeliBulletDamage(Bullet HeliBullets[], Player& P, Shield& S, Rectangle PlayerR) {
    for (int i = 0; i < MAX_BULLETS; i++) {

        if (HeliBullets[i].active) {


            HeliBullets[i].BCollider = { (float)HeliBullets[i].x, (float)HeliBullets[i].y,
                (float)HeliBullets[i].BWidth, (float)HeliBullets[i].BHeight };

            if (CheckCollisionRecs(HeliBullets[i].BCollider, PlayerR)) {

                if (S.active && S.health > 0) {

                    S.health -= 7;
                    if (S.health <= 0) {


                        S.active = false;
                    }
                }
                else {

                    P.PHealth -= 7;

                }
                HeliBullets[i].active = false;
            }
            else if (HeliBullets[i].y >= ScreenHeight - 50 &&
                HeliBullets[i].x >= P.x[0] - 100 &&
                HeliBullets[i].x <= P.x[0] + 100) {
                if (S.active && S.health > 0) {
                    S.health -= 4;
                    if (S.health <= 0) {
                        S.active = false;
                    }
                }
                else {
                    P.PHealth -= 4;
                }
                HeliBullets[i].active = false;
            }
        }
    }
}

void DrawLevelInfo(int level)
{
    DrawText(TextFormat("LEVEL: %d", level), ScreenWidth - 200, 45, 20, WHITE);
}


// **********************MAINS************
int main()
{
    srand(time(0));
    InitAudioDevice();
    bool changeBGonce = false;
    int level = 1;
    int ScreenTimer = 120;
    Player P;
    Bullet bullets[MAX_BULLETS] = {};
    Bullet enemyManBullets[MAX_BULLETS] = {};
    Bullet enemyBullets[MAX_BULLETS] = {};
    Bullet HeliBullets[MAX_BULLETS] = {};
    Music bgIntro = LoadMusicStream("bgIntro.mp3");
    Music bgMusic = LoadMusicStream("bgMusic.mp3");
    Sound BulletSound = LoadSound("Bullet_Sound.mp3");
    SetMusicVolume(bgIntro, 1.0f);
    SetMusicVolume(bgMusic, 1.0f);
    SetSoundVolume(BulletSound, 1.0f);

    Enemy E, EnemyMan, Helicopter;  //Can be array
    E.loadImage = LoadEnemyImage;
    EnemyMan.loadImage = LoadEnemyManImage;
    Helicopter.loadImage = LoadHelicopterImage;
    P.isAlive = true;
    P.PHealth = 100.0;
    BG b1, b2;
    Texture2D bg1, bg2, Title;
    bool GameStart = false;
    int score = 0;

    Shield S;
    S.health = 100.0f;
    S.active = false;


    InitWindow(ScreenWidth, ScreenHeight, "METAL SLUG");

    SetTargetFPS(60);

    E.loadImage(E);
    EnemyMan.loadImage(EnemyMan);
    Helicopter.loadImage(Helicopter);
    E.timer = 0;
    EnemyMan.timer = 0;
    E.bulletFrame = 0;
    EnemyMan.bulletFrame = 0;
    Helicopter.timer = 0;
    Helicopter.bulletFrame = 0;



    LoadPlayerBulletImages(bullets, P, "Bullet.png");
    LoadEnemyBulletImages(enemyBullets, "eBullet1.png");
    LoadEnemyBulletImages(enemyManBullets, "eBullet2.png");
    LoadEnemyBulletImages(HeliBullets, "eBullet3.png");

    S.SImage = LoadTexture("Shield.png");
    LoadPlayerImages(P);




    bg1 = LoadTexture("Back1.png");
    bg1.height = ScreenHeight, bg1.width = ScreenWidth;
    bg2 = LoadTexture("Back2.png");
    bg2.height = ScreenHeight, bg2.width = ScreenWidth;
    Title = LoadTexture("Title.png");

    E.x = ScreenWidth, E.y = ScreenHeight - E.EHeight + 25;
    EnemyMan.x = ScreenWidth, EnemyMan.y = ScreenHeight - EnemyMan.EHeight + 25;
    Helicopter.x = ScreenWidth, Helicopter.y = Helicopter.EHeight - 20;

    int TotalPlayerHeight = P.PHeight[0] + P.PHeight[1];
    int TotalPlayerWidth = P.PWidth[0];

    b1.ri = 0, b1.ci = 0;
    b2.ri = 0, b2.ci = ScreenWidth;

    int FrameCount = 0;
    bool ShootHappen = false;
    int ShootFrame = 0;
    int LegFrame = 1;

    float PSpeed = 0;
    bool Jump = false;

    PlayMusicStream(bgIntro);
    for (int i = 1; i <= 100; i++)  // Title Show
    {
        UpdateMusicStream(bgIntro);
        Title.height = 4 * i, Title.width = 6 * i;
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Title, ScreenWidth / 2 - Title.width / 2, 20, WHITE);
        EndDrawing();
    }

    while (!WindowShouldClose() && !GameStart) //Start Menu
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Title, ScreenWidth / 2 - Title.width / 2, 20, WHITE);
        DrawText("PRESS ENTER TO START", ScreenWidth / 2 - 180, ScreenHeight / 2 + 150, 30, WHITE);
        if (IsKeyPressed(KEY_ENTER))
        {
            StopMusicStream(bgIntro);
            PlayMusicStream(bgMusic);
            GameStart = true;
        }
        EndDrawing();
    }
    while (!WindowShouldClose())
    {
        ShootFrame++;
        if (ShootFrame >= 60)
        {
            ShootFrame = 0;
        }
        UpdateMusicStream(bgMusic);
        BeginDrawing();
        DrawTexture(bg1, b1.ci, b1.ri, WHITE);
        DrawTexture(bg2, b2.ci, b2.ri, WHITE);


        //COLLIDER CHECKERS

        //DrawRectangle(P.x[0], P.y[0], TotalPlayerWidth-30, TotalPlayerHeight-130, WHITE);  //Player collider checker
        //DrawRectangle(E.x+15,E.y, E.EWidth-30,E.EHeight, WHITE); //Enemy Collider Checker
        //DrawRectangle(EnemyMan.x, EnemyMan.y, EnemyMan.EWidth-10, EnemyMan.EHeight, WHITE);  //EnemyMan Collider Checker
        //DrawRectangle(Helicopter.x+16, Helicopter.y, Helicopter.EWidth+30, Helicopter.EHeight-80, WHITE); //Helicopter Collider Checker


        DrawTexture(P.PImage[LegFrame], P.x[1], P.y[1], WHITE);


        if (ShootHappen)
        {
            DrawTexture(P.PImage[6], P.x[2], P.y[2], WHITE);
        }
        else
        {
            DrawTexture(P.PImage[0], P.x[0], P.y[0], WHITE);
        }
        if (ShootFrame == 8)
        {
            ShootHappen = false;
        }

        DrawTexture(E.EImage, E.x, E.y, WHITE);
        DrawTexture(EnemyMan.EImage, EnemyMan.x, EnemyMan.y, WHITE);


        ShieldHealth(S);

        if (score >= 15 and level == 1 and changeBGonce == false) {
            changeBGonce = true;
            bg1 = LoadTexture("bg3.png");
            bg2 = LoadTexture("bg4.png");
            bg1.height = ScreenHeight;
            bg1.width = ScreenWidth;
            bg2.height = ScreenHeight;
            bg2.width = ScreenWidth;
        }
        if (score >= 15 and level == 1) {
            level = 2;
            ScreenTimer = 120;

            while (ScreenTimer > 0) {

                UpdateMusicStream(bgMusic);
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("LEVEL 2", ScreenWidth / 2 - 100, ScreenHeight / 2 - 50, 40, WHITE);
                DrawText("Use E to destroy the Helicopter!", ScreenWidth / 2 - 250, ScreenHeight / 2 + 20, 30, WHITE);

                ScreenTimer--;
                EndDrawing();
            }

            RespawnEnemy(E);
            RespawnEnemy(EnemyMan);
            RespawnHelicopter(Helicopter);
        }

        DrawLevelInfo(level);

        E.x--;
        EnemyMan.x--;
        if (level >= 2) {
            E.x--;
            EnemyMan.x--;
            Helicopter.x--;

        }

        EnemyBulletShooting(E, enemyBullets);
        EnemyBulletShooting(EnemyMan, enemyManBullets);

        if (level >= 2) {
            HelicopterBulletShooting(Helicopter, HeliBullets);
            DrawTexture(Helicopter.EImage, Helicopter.x, Helicopter.y, WHITE);
            HeliBulletDamage(HeliBullets, P, S, P.PCollider);
            Helicopter.x--;
        }



        P.PCollider = { (float)P.x[0], (float)P.y[0], (float)TotalPlayerWidth - 30, (float)TotalPlayerHeight - 130 };

        if (IsKeyDown(KEY_LEFT_SHIFT) && S.health > 0) {
            S.active = true;
        }
        else
            S.active = false;

        if (S.active && S.health > 0) {
            DrawTexture(S.SImage, P.x[0] + 25, P.y[0] - 10, WHITE);
        }

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (enemyBullets[i].active) {

                enemyBullets[i].BCollider = { (float)enemyBullets[i].x, (float)enemyBullets[i].y,
                    (float)enemyBullets[i].BWidth, (float)enemyBullets[i].BHeight };

                if (CheckCollisionRecs(enemyBullets[i].BCollider, P.PCollider)) {
                    if (S.active && S.health > 0) {
                        S.health -= 5;
                        enemyManBullets[i].active = false;

                        if (S.health <= 0) S.active = false;
                    }
                    else {
                        P.PHealth -= 3;
                    }
                    enemyBullets[i].active = false;
                }
            }
            if (enemyManBullets[i].active) {

                enemyManBullets[i].BCollider = { (float)enemyManBullets[i].x, (float)enemyManBullets[i].y,
                    (float)enemyManBullets[i].BWidth, (float)enemyManBullets[i].BHeight };

                if (CheckCollisionRecs(enemyManBullets[i].BCollider, P.PCollider)) {
                    if (S.active && S.health > 0) {
                        S.health -= 5;
                        enemyManBullets[i].active = false;

                        if (S.health <= 0) S.active = false;
                    }
                    else {
                        P.PHealth -= 3;
                    }
                    enemyBullets[i].active = false;
                }
            }
        }

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
            if (level == 2) {
                E.x -= 1;
                EnemyMan.x -= 1;
                Helicopter.x -= 3;
            }
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
        P.y[2] += PSpeed;
        PSpeed += 0.5;

        if (P.y[0] >= ScreenHeight - P.PHeight[0] + 25)
        {
            P.y[0] = ScreenHeight - P.PHeight[0] + 25;
            P.y[1] = ScreenHeight - P.PHeight[1] - 25;
            P.y[2] = ScreenHeight - P.PHeight[0] - 15;
            PSpeed = 0;
            Jump = false;
        }

        Rectangle PlayerR = { (float)P.x[0],(float)P.y[0],(float)TotalPlayerWidth - 30,(float)TotalPlayerHeight - 130 };
        Rectangle EnemyR = { (float)E.x + 15,(float)E.y,(float)E.EWidth - 30,(float)E.EHeight };
        Rectangle EnemyManR = { (float)EnemyMan.x,(float)EnemyMan.y,(float)EnemyMan.EWidth - 10,(float)EnemyMan.EHeight };
        Rectangle HelicopterR = { (float)Helicopter.x + 16,(float)Helicopter.y,(float)Helicopter.EWidth + 30,(float)Helicopter.EHeight - 80 };

        if (IsKeyPressed(KEY_SPACE))
        {
            ShootHappen = true;
            PlaySound(BulletSound);

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active == false)
                {
                    bullets[i].active = true;
                    bullets[i].x = P.x[0] + P.PWidth[0] - 30;
                    bullets[i].y = P.y[0] + 7;
                    bullets[i].vel_y = 0;
                    break;
                }
            }
        }

        if (IsKeyPressed(KEY_E))
        {
            ShootHappen = true;
            PlaySound(BulletSound);

            for (int i = 0; i < MAX_BULLETS; i++)
            {
                if (bullets[i].active == false)
                {
                    bullets[i].active = true;
                    bullets[i].x = P.x[0] + P.PWidth[0] - 30;
                    bullets[i].y = P.y[0] + 7;

                    float xChange = Helicopter.x - bullets[i].x;
                    float yChange = Helicopter.y - bullets[i].y;

                    bullets[i].vel_x = xChange / 40;
                    bullets[i].vel_y = yChange / 40;
                    break;
                }
            }
        }

        PlayerBulletAndEnemyCollisionCheck(bullets, score, E, EnemyMan, Helicopter, EnemyR, EnemyManR, HelicopterR);


        if (P.PHealth <= 0) {
            DrawText("GAME OVER", ScreenWidth / 2 - 100, ScreenHeight / 2, 40, RED);
            EndDrawing();
            while (!IsKeyPressed(KEY_ESCAPE)) {
                BeginDrawing();
                ClearBackground(BLACK);
                DrawText("GAME OVER", ScreenWidth / 2 - 100, ScreenHeight / 2 - 100, 40, RED);
                DrawText("Press escape to exit", ScreenWidth / 2 - 120, ScreenHeight / 2 - 120 + 60, 30, WHITE);

                EndDrawing();

            }

            break;
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

        HealthBar(P);

        EndDrawing();

    }
    CloseWindow();
    return 0;
}
