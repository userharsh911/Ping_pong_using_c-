#include <raylib.h>
#include <bits/stdc++.h>
#include "iostream"
using namespace std;
// ----------------------------------------------------------------------------------------------
/*
    PLAYER CLASS START 
*/
class Game{
    protected:
    int score;
    static bool run;
    static bool over;
    public:
    void game_over(int screen_width, int screen_height){
        if(over) DrawText("Game Over Press R to restart ", screen_width / 2 - 400, screen_height / 2 - 25, 50, RED);
        if(IsKeyPressed(KEY_R) && over) over = false;
    }
    void start(int screen_width, int screen_height){
        if(!run) DrawText("Press Enter to Start ", screen_width / 2 - 200, screen_height / 2 - 25, 50, GREEN);
        if(IsKeyPressed(KEY_ENTER) && !run) run = true;
    }
    void increment_score(){score++;}
};
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
            Vector2 v = {x, y};
            return v;
        }
        Vector2 get_dimensions(){
            Vector2 v = {b,l};
            return v;
        }
        void print_score(int screen_width, int screen_height){
            string text = to_string(score);
            if(!run) DrawText(&text[0], screen_width / 2 - 100, screen_height / 2 - 25, 50, RED);
        }
};
class Player_left : public Player{
    public:
        Player_left(int l, int b, int screen_height) : Player(l, b){x = 0, y = (screen_height / 2) - (l / 2), speed = 10, color = RED;}
        void movement(int screen_height){
            if(run && !over && IsKeyDown(KEY_W) && y > 0){y -= speed;}
            if(run && !over && IsKeyDown(KEY_S) && y < screen_height - l){y += speed;}
        }
};
class Player_right : public Player{
    public:
        Player_right(int l, int b, int screen_width, int screen_height) : Player(l, b){x = screen_width - b, y = (screen_height / 2) - (l / 2), speed = 10, color = BLUE;}
        void movement(int screen_height){
            if(run && !over && IsKeyDown(KEY_UP) && y > 0){y -= speed;}
            if(run && !over && IsKeyDown(KEY_DOWN) && y < screen_height - l){y += speed;}
        }
};

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
        Ball(int radius, int speed_x, int speed_y, Color color, int screen_width) : radius(radius), x(x = screen_width / 2 - radius), y(radius + 1), speed_x(speed_x), speed_y(speed_y), color(color){}
        void coordinates(int x, int screen_w){
            if(x < radius){
                x = radius;
                return;
            }
            if(x >= screen_w - radius * 2){
                this->x = screen_w - radius * 2;
                speed_x *= -1;
                return;
            }
            this->x = x;
        }
        void update(){
            if(run && !over){
                y += speed_y;
                x += speed_x;
            }
        }
        void draw(){if(run && !over) DrawCircle(x, y, radius, color);}
        void bounce(int screen_width, int screen_height){
            if(x >= screen_width - radius || x <= radius){
                cout<<"Game over: "<<endl;
                x = (screen_width / 2) - (radius / 2);
                y = (screen_height / 2) - (radius / 2);
                over = true;
            }
            if(y >= screen_height - radius || y <= radius) speed_y *= -1;
        }
        void collision_right(Player_right &player){
            // if((this->x >= ball.x - this->radius && this->x <= ball.x + ball.radius && this->y >= ball.y - this->radius && this->y <= ball.y + ball.radius)){
            //     this->speed_x *= -1;
            //     ball.speed_x *= -1;
            // }

            if(x >= player.get_coordinates().x - radius && y >= player.get_coordinates().y - radius && y <= player.get_coordinates().y + player.get_dimensions().y){
                cout<<"collsion right :"<<endl;
                speed_x = abs(speed_x) * -1;
                player.increment_score();
            }
        }
        void collision_left(Player_left &player){
            // if((this->x >= ball.x - this->radius && this->x <= ball.x + ball.radius && this->y >= ball.y - this->radius && this->y <= ball.y + ball.radius)){
            //     this->speed_x *= -1;
            //     ball.speed_x *= -1;
            // }

            if(x <= radius + player.get_dimensions().x && y >= player.get_coordinates().y - radius && y <= player.get_coordinates().y + player.get_dimensions().y){
                cout<<"collsion left :"<<endl;
                speed_x = abs(speed_x);
                player.increment_score();
            }
        }
};
/*
    BALL CLASS END 
*/

bool Game::run = false;
bool Game::over = false;
int main() {
    cout<<"Starting the game...";
    const int screen_width = 1080;
    const int screen_height = 720;

    srand(time(0));
    Game g;
    Ball ball_1(20, 8, 8, RAYWHITE, screen_width);
    Player_left player_left(100,20,screen_height);
    Player_right player_right(100,20,screen_width,screen_height);

    InitWindow(screen_width, screen_height, "PING PONG");
    SetTargetFPS(60);
    while(!WindowShouldClose()){
        g.start(screen_width, screen_height);
        ball_1.update();

        player_left.movement(screen_height);
        player_right.movement(screen_height);
        
        ball_1.collision_right(player_right);
        ball_1.collision_left(player_left);
        ball_1.bounce(screen_width, screen_height);
        
        g.game_over(screen_width, screen_height);
        BeginDrawing();

        ClearBackground(BLACK);

        ball_1.draw();
        player_right.draw();
        player_left.draw();

        EndDrawing();
    }
    CloseWindow();
}