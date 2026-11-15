#include <raylib.h>
#include "iostream"
using namespace std;
class Game{
    protected:
    static bool run;
    static bool over;
    static string winner;
    public:
    void game_over(Sound &game_over, Music &background_music){
        if(over) DrawText(&winner[0], GetScreenWidth() / 2 - 400, GetScreenHeight() / 2 - 25, 50, RED);
        if(IsKeyPressed(KEY_R) && over) over = false, StopSound(game_over), PlayMusicStream(background_music);
    }
    void start(){
        if(!run) DrawText("Press Enter to Start ", GetScreenWidth() / 2 - 270, GetScreenHeight() / 2 - 25, 50, BLACK);
        if(IsKeyPressed(KEY_ENTER) && !run) run = true;
    }
};
// ----------------------------------------------------------------------------------------------
/*
    PLAYER CLASS START 
*/
// ----------------------------------------------------------------------------------------------
class Player : public Game{
    protected:
    int l;
    int b;
    int x;
    int y;
    int speed;
    Color color;
    public:
    Player(int l, int b) : l(l), b(b){}
    void draw(){DrawRectangle(x, y, b, l, color);}
    Vector2 get_coordinates(){
        Vector2 v = {static_cast<float>(x), static_cast<float>(y)};
        return v;
    }
    Vector2 get_dimensions(){
        Vector2 v = {static_cast<float>(b), static_cast<float>(l)};
        return v;
    }
};
class Player_left : public Player{
    public:
        Player_left(int l, int b) : Player(l, b){x = 0, y = (GetScreenHeight() / 2) - (l / 2), speed = 10, color = RED;}
        void movement(){
            if(run && !over && IsKeyDown(KEY_W) && y > 0){y -= speed;}
            if(run && !over && IsKeyDown(KEY_S) && y < GetScreenHeight() - l){y += speed;}
        }
};
class Player_right : public Player{
    public:
        Player_right(int l, int b) : Player(l, b){x = GetScreenWidth() - b, y = (GetScreenHeight() / 2) - (l / 2), speed = 10, color = BLUE;}
        void movement(){
            if(run && !over && IsKeyDown(KEY_UP) && y > 0){y -= speed;}
            if(run && !over && IsKeyDown(KEY_DOWN) && y < GetScreenHeight() - l){y += speed;}
        }
};
// ----------------------------------------------------------------------------------------------
/*
    PLAYER CLASS END 
*/
// ----------------------------------------------------------------------------------------------
/*
    BALL CLASS START 
*/
// ----------------------------------------------------------------------------------------------
class Ball : public Game{
    private:
        int radius;
        int x;
        int y;
        int speed_x;
        int speed_y;
        Color color;
    public:
        Ball(int radius, int speed_x, int speed_y, Color color) : radius(radius), x(GetScreenWidth() / 2 - radius), y(radius + 1), speed_x(speed_x), speed_y(speed_y), color(color){}
        void update(){
            if(run && !over){
                y += speed_y;
                x += speed_x;
            }
        }
        void draw(){if(run && !over) DrawCircle(x, y, radius, color);}
        void bounce(Sound &game_over, Music & background_music){
            if(x >= GetScreenWidth() - radius || x <= radius){
                x = (GetScreenWidth() / 2) - (radius / 2);
                    over = true;
                StopMusicStream(background_music);
                PlaySound(game_over);
            }
            if(y >= GetScreenHeight() - radius || y <= radius) speed_y *= -1;
        }
        void collision_right(Player_right &player, Sound &hit){
            if(x >= player.get_coordinates().x - radius && y >= player.get_coordinates().y - radius && y <= player.get_coordinates().y + player.get_dimensions().y && x < GetScreenWidth() - radius){
                speed_x = abs(speed_x) * -1;
                PlaySound(hit);
                winner = "BLUE wins Press R to restart";
            }
        }
        void collision_left(Player_left &player, Sound &hit){
            if(x <= radius + player.get_dimensions().x && y >= player.get_coordinates().y - radius && y <= player.get_coordinates().y + player.get_dimensions().y && x > radius){
                speed_x = abs(speed_x);
                PlaySound(hit);
                winner = "RED wins Press R to restart";
            }
        }
};
/*
    BALL CLASS END 
*/

bool Game::run = false;
bool Game::over = false;
string Game::winner = "RED wins Press R to restart";
int main() {
    cout<<"Starting the game...";
    const int screen_width = 1080;
    const int screen_height = 720;
    InitWindow(screen_width, screen_height, "PING PONG");

    Game game;
    Ball ball_1(20, 8, 8, DARKGREEN);
    Player_left player_left(100,20);
    Player_right player_right(100,20);

    InitAudioDevice();

    Sound hit = LoadSound("./src/audio/hit.mp3");
    Sound game_over = LoadSound("./src/audio/game_over.mp3");
    Music background_music = LoadMusicStream("./src/audio/background_music.mp3");

    float pan = 0.0f;
    SetMusicPan(background_music, pan);

    float volume = 0.7f;
    SetMusicVolume(background_music, volume);
    PlayMusicStream(background_music);

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        UpdateMusicStream(background_music);

        ball_1.update();
        player_left.movement();
        player_right.movement();


        ball_1.collision_right(player_right, hit);
        ball_1.collision_left(player_left, hit);
        ball_1.bounce(game_over, background_music);
        
        BeginDrawing();

        game.start();
        game.game_over(game_over, background_music);
        
        ClearBackground(BEIGE);

        ball_1.draw();
        player_right.draw();
        player_left.draw();

        EndDrawing();
    }
    UnloadSound(hit);
    UnloadSound(game_over);
    UnloadMusicStream(background_music);
    CloseAudioDevice();
    CloseWindow();
}