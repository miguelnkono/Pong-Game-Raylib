#include "raylib.h"
#include <iostream>

using namespace std;

// Couleur
Color Vert = Color{38, 185, 154, 255};
Color Vert_noir = Color{20, 160, 133, 255};
Color Vert_claire = Color{129, 204, 184, 255};
Color Jaune = Color{243, 213, 91, 255};

int player_score   = 0;
int cpu_score      = 0;

class Ball{
public:
    float x, y;
    int vitesseX, vitesseY;
    int rayon;

    void Draw()
    {
        DrawCircle(x, y, rayon, Jaune);
    }

    void Update()
    {
        x += vitesseX;
        y += vitesseY;

        if( y + rayon >= GetScreenHeight() || y - rayon <= 0)
        {
            vitesseY *= -1;
        }

        if( x + rayon >= GetScreenWidth())
        {
            cpu_score++;
            Reinitialise();
        }
        if(x - rayon <= 0)
        {
            player_score++;
            Reinitialise();
        }
    }

    void Reinitialise()
    {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int choix_vitesse[2] = {-1, 1};
        vitesseX *= choix_vitesse[GetRandomValue(0, 1)];
        vitesseY *= choix_vitesse[GetRandomValue(0, 1)];
    }
};

class Paddle{
protected:
    void LimiteMouvement()
    {
        if( y <= 0){
            y = 0;
        }
        if ( y + longueur >= GetScreenHeight())
        {
            y = GetScreenHeight() - longueur;
        }
    }

public:
    float x, y;
    float largeur, longueur;
    float vitesseY;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, largeur, longueur}, 0.8, 0, WHITE);
    }
    void Update()
    {
        if(IsKeyDown(KEY_UP))
        {
            y = y - vitesseY;
        }
        if(IsKeyDown(KEY_DOWN))
        {
            y = y + vitesseY;
        }
        LimiteMouvement();
    }
};


class CpuPaddle: public Paddle{
public:
    void Update(float ballY)
    {
        if(y + longueur/2 > ballY)
        {
            y = y - vitesseY;
        }
        if(y + longueur/2 <= ballY)
        {
            y = y + vitesseY;
        }
        LimiteMouvement();
    }

};


Ball        ball;
Paddle      player;
CpuPaddle   cpu;

int main()
{
    const int ecranX = 1200;
    const int ecranY = 600;

    // Balle
    ball.rayon      = 10;
    ball.x          = ecranX / 2;
    ball.y          = ecranY / 2;
    ball.vitesseX   =7;
    ball.vitesseY   =7;

    // Barre
    player.largeur      = 25;
    player.longueur     = 120;
    player.x            = ecranX - player.largeur - 10;
    player.y            = ecranY / 2 - player.longueur / 2;
    player.vitesseY     = 6;

    // CPU
    cpu.longueur        = 120;
    cpu.largeur         = 25;
    cpu.x               = 10;
    cpu.y               = ecranY / 2 - cpu.longueur / 2;
    cpu.vitesseY        = 6;

//    cout << "Start the game" << endl;

    SetTargetFPS(60);

    InitWindow(ecranX, ecranY, "Jeu de Pong [Pong - Game]");

    while(WindowShouldClose() == false)
    {
        // Gestion d'evenement


        // Mise a jours des variables
        ball.Update();
        player.Update();
        cpu.Update(ball.y);

        // Collision
       if( CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.rayon, Rectangle{player.x,player.y, player.largeur, player.longueur}))
       {
           ball.vitesseX *= -1;
       }

       if( CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.rayon, Rectangle{cpu.x, cpu.y, cpu.largeur, cpu.longueur}))
        {
            ball.vitesseX *= -1;
        }
        // Dessin
        BeginDrawing();

            // Clean
            ClearBackground(Vert_noir);

            // Header

            DrawRectangle(ecranX/2, 0, ecranX/2, ecranY, Vert);

            DrawCircle(ecranX/2, ecranY/2, 150, Vert_claire);
            // Ligne
            DrawLine(ecranX/2, 0, ecranX/2, ecranY, WHITE);

            // Cercle
            ball.Draw();
            // Rectangle
            cpu.Draw();
            player.Draw();

            // Afficher du texte
            DrawText(TextFormat("%i", cpu_score), ecranX/4 - 20, 20, 80, WHITE);
            DrawText(TextFormat("%i", player_score), 3 * ecranX/4 - 20, 20, 80, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
