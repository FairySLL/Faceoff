#pragma once
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>

using namespace std;

//Window
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;

//Players
const int PLAYER_SIZE = 50;

const int PLAYER1_X = 100;
const int PLAYER1_Y = 150;

const int PLAYER2_X = 450;
const int PLAYER2_Y = 150;

//Health bars
const int HP_WIDTH = 100;
const int HP_HEIGHT = 20;
const int PLAYER1_HP_X = 75;
const int PLAYER1_HP_Y = 100;
const int PLAYER2_HP_X = 425;
const int PLAYER2_HP_Y = 100;

//Colors
const SDL_Color NORMAL_COLOR = { 80,80,200,255 };
const SDL_Color HOVER_COLOR = { 100,100,255,255 };
const SDL_Color CLICKED_COLOR = { 200,60,60,255 };


enum GameState {
    Turn,
    Text,
    GameOver
};

struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;

};

class Player
{
private:
    SDL_FRect rect;
	SDL_FRect hpBar;
	SDL_Color color;
    SDL_Renderer* renderer;
    int currentHp;
    int maxHp;
    string name;

public:
    Player(SDL_FRect rect, SDL_FRect hpBar, SDL_Color color, SDL_Renderer* renderer, int maxHp, string name);
    //~Player();

    int getHp() const { return currentHp; }
    void setHp(int health) { currentHp = health; }
    void takeDmg(int damage) { currentHp -= damage; }
    void renderPlayer() const;
    void renderHPBar() const; 

    const string* who() const { return &name; }

    const SDL_FRect* getHpBar() const { return &hpBar; }

    const SDL_Color* getColor() const { return &color; }

    const SDL_FRect* getRect() const { return &rect; }

    const SDL_Renderer* getRender() const { return renderer; }

   



};

class Button {
private:
    SDL_FRect rect;
    SDL_Color colorNormal;
    SDL_Color colorHover;
    SDL_Color colorClicked;
    string text;
    TTF_Font* font;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;
    bool isClicked;
    bool isHovered;

public:
    // Constructor & Destructor
    Button(float x, float y, float w, float h, SDL_Color colorNormal, SDL_Color colorHover, SDL_Color colorClicked, const string& text, TTF_Font* font, SDL_Renderer* renderer);
    ~Button();

    //Getter * Setters, ***** func's defined here *****
    // 
    //isClicked
    bool getIsClicked() const { return isClicked; }
    void setIsClicked(bool value) { isClicked = value; }

    //isHovered
    bool getIsHovered() const { return isHovered; }
    void setIsHovered(bool value) { isHovered = value; }

    //colorNormal
    SDL_Color getColorNormal() const { return colorNormal; }

    //font
    

    // Member functions
    void render(SDL_Renderer* renderer) const;
    void handleEvent(const SDL_Event& event);
    void setText(const string& newText);
};

class Textbox {
private:
    SDL_FRect rect;
    SDL_Color colorRect;
    SDL_Color colorBorder;
    SDL_Color colorText;
    string text;
    TTF_Font* font;
    SDL_Texture* textTexture;
    SDL_Renderer* renderer;
    SDL_FRect borderTop;
    SDL_FRect borderBottom;
    SDL_FRect borderRight;
    SDL_FRect borderLeft;
    float paddingX = 4.0f;
    float paddingY = 4.0f;
    float borderThickness = 2.0f;

public:
    Textbox(SDL_FRect rect, SDL_Color colorRect, SDL_Color colorBorder, SDL_Color colorText, const string& text, TTF_Font* font, 
             SDL_Renderer* renderer);
    ~Textbox();

    //set the border
    void setBorder();
    void drawTextbox();
    void setTextBox(const string& newText);
        

};


bool PointInRectF(float px, float py, const SDL_FRect& rect);

