#include "Faceoff.h"
#include <SDL3_ttf/SDL_ttf.h>



int main(int argc, char* argv[])
{
	if (!TTF_Init()) {
		SDL_Log("Failed to init SDL_ttf: %s", SDL_GetError());
	}

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
	SDL_Window* window = SDL_CreateWindow("title", 
		WINDOW_WIDTH, 
		WINDOW_HEIGHT, 
		0
		);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

									//R, G, B, Alpha;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //MAGIC NUMBER ALERT !!!
	
	SDL_RenderClear(renderer);



	Player player1 = {
	{ PLAYER1_X, PLAYER1_Y, PLAYER_SIZE, PLAYER_SIZE },  
	{ PLAYER1_HP_X, PLAYER1_HP_Y, HP_WIDTH, HP_HEIGHT }, 
	{ 0, 0, 255, 255 }, 
	renderer,
	100,
	"Player 1"
	};
	
	Player player2 = {
	{ PLAYER2_X, PLAYER2_Y, PLAYER_SIZE, PLAYER_SIZE },
	{ PLAYER2_HP_X, PLAYER2_HP_Y, HP_WIDTH, HP_HEIGHT },
	{ 255, 0, 0, 255 },
	renderer,
	100, 
	"Player 2"
	};

	//Dimensions for text boxes
	SDL_FRect textboxRect{
		WINDOW_WIDTH * (1.0f / 24.0f),
		WINDOW_HEIGHT * (2.0f / 3.0f),
		WINDOW_WIDTH * (1.0f - 1.0f / 12.0f),
		WINDOW_HEIGHT * (1.0f / 3.0f)
	};

	SDL_Color blue = { 0,0,255,255 }; //MAGIC NUMBER ALERT !!!! Probably define these as consts in header?
	SDL_Color lightBlue = { 0,0,150,255 };
	SDL_Color red = { 255,0,0,255 };
	SDL_Color white = { 255,255,255,255 };
	SDL_Color grey = { 94,92,92,255 };

	TTF_Font* font = TTF_OpenFont("C:/dev/showdownClone/1stSDLWindow/assets/fonts/RougeVintage.ttf", 24); //MAGIC NUMBER ALERT !!!!
	if (!font) {
		SDL_Log("Failed to load font: %s", SDL_GetError());
	}
	string attackTxt1 = "Punch";
	string attackTxt2 = "Kick";

	Button p1attackButtonA(60, 250, 120, 80, blue, lightBlue, red, attackTxt1, font, renderer); //MAGIC NUMBER ALERT !!!!!!
	Button p1attackButtonB(200, 250, 120, 80, blue, lightBlue, red, attackTxt2, font, renderer); //MAGIC NUMBER ALERT !!!!!!

	Button p2attackButtonA(470, 250, 120, 80, blue, lightBlue, red, attackTxt1, font, renderer); //MAGIC NUMBER ALERT !!!!!!
	Button p2attackButtonB(330, 250, 120, 80, blue, lightBlue, red, attackTxt2, font, renderer); //MAGIC NUMBER ALERT !!!!!!

	string boxText = "Hello world!";
	Textbox mainBox(textboxRect, grey, blue, white, boxText, font, renderer); 

	mainBox.setTextBox(boxText);

	p1attackButtonA.setText(attackTxt1);
	p1attackButtonB.setText(attackTxt2);

	p2attackButtonA.setText(attackTxt1);
	p2attackButtonB.setText(attackTxt2);
	
	player1.renderPlayer();
	player2.renderPlayer();

	p2attackButtonA.render(renderer);
	p2attackButtonB.render(renderer);

	SDL_RenderPresent(renderer);
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	GameState gameState = GameState::Turn;
	int currentTurn = 1;
	//Start game loop
	bool running = true;
	while (running)
	{
		SDL_Event event{ 0 };
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_EVENT_QUIT)
				running = false;


			switch (gameState)
			{
			case GameState::Turn:
				if (currentTurn == 1)
				{
					p1attackButtonA.handleEvent(event);
					p1attackButtonB.handleEvent(event);

					if (p1attackButtonA.getIsClicked())
					{
						cout << "Button p1A clicked!\n";

						player2.takeDmg(25);
						cout << "Player2 hp: " << player2.getHp() << "\n";

						currentTurn = 2;

					}

					else if (p1attackButtonB.getIsClicked())
					{
						cout << "Button p1B clicked!\n";

						player2.takeDmg(20);
						cout << "Player2 hp: " << player2.getHp() << "\n";

						currentTurn = 2;

					}

				}
				else if (currentTurn == 2)
				{

					p2attackButtonA.handleEvent(event);
					p2attackButtonB.handleEvent(event);

					if (p2attackButtonA.getIsClicked())
					{
						cout << "Button 1 clicked!\n";

						player1.takeDmg(25);
						cout << "Player2 hp: " << player1.getHp() << "\n";

						currentTurn = 1;

					}

					else if (p2attackButtonB.getIsClicked())
					{
						cout << "Button 2 clicked!\n";

						player1.takeDmg(20);
						cout << "Player2 hp: " << player1.getHp() << "\n";

						currentTurn = 1;

					}
				}

				if (player1.getHp() <= 0)
				{
					gameState = GameState::GameOver;
				}
				else if (player2.getHp() <= 0)
				{
					gameState = GameState::GameOver;
				}

				break;

			case GameState::Text:
				{
					SDL_Delay(1000);
			}

			case GameState::GameOver:
			{
				if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_KEY_DOWN)
				{
					running = false;
				}
				break;
			}
			}
		}
		

		// RENDER 

		if (gameState == GameState::Text)
		{
		
		}

		if (gameState == GameState::GameOver)
		{
			boxText = "Player dead! Game over.";
			mainBox.setTextBox(boxText);
			mainBox.drawTextbox();
		}

		if (gameState == GameState::Turn) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //MAGIC NUMBER ALERT !!!!
			SDL_RenderClear(renderer);

			player1.renderPlayer();
			player1.renderHPBar();
			player2.renderPlayer();
			player2.renderHPBar();
			//mainBox.drawTextbox();

			if (currentTurn == 1) {
				p1attackButtonA.render(renderer);
				p1attackButtonB.render(renderer);
			}

			if (currentTurn == 2) {
				p2attackButtonA.render(renderer);
				p2attackButtonB.render(renderer);
			}
		}

		SDL_RenderPresent(renderer);
	}		

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		return 1;

	SDL_Quit();

	


	return 0;
}

