#include <SDL.h>
#include <SDL_image.h>//useful image load and processing functions. Supports lots of image types e.g png,jpg
#include <SDL_ttf.h> //loads up ttf font files and helps render text to texture
#include <iostream>
#include <string>
#include "Animation.h"
#include "GameObject.h"
#include "Hero.h"
#include "Wall.h"
#include <list>
#include "KeyboardHandler.h"
#include "MouseHandler.h"
#include "GameControllerHandler.h"
#include "Ball.h"
#include <stdlib.h> //rand and srand
#include <time.h> //get milliseconds since start of year

using namespace std;


void renderText(TTF_Font* font, SDL_Color textColour, string text, SDL_Renderer* renderer, SDL_Texture* &texture) {
	//clean up memory, before rebuild
	if (texture != NULL)
		SDL_DestroyTexture(texture);
	
	//create surface(image in ram) using font, colour and text
	SDL_Surface* textSurface = TTF_RenderText_Blended(font, text.c_str(), textColour);
	//convert surface to texture
	texture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//cleanup surface
	SDL_FreeSurface(textSurface);
}

//for SDL to work, need old 'main' signature, not void main()
int main(int argc, char **argv)
{
	//SET random number series seed value (changes which set of random numbers are returned from rand)
	srand(time(NULL));//time(NULL) gives milliseconds since the start of this year, so should be different each game run



	//initialise SDL and its subsystems
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		return -1; //exit program early
	}
	else
	{
		cout << "SDL initialised!!!" << endl;
	}
	//Initialise SDL image
	if (!(IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG)))
	{
		//if didn't work, report error
		cout << "SDL IMAGE ERROR: " << IMG_GetError() << endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	if (TTF_Init() != 0)
	{
		//failed!
		cout << "TTF FAILED: " << TTF_GetError() << endl;
		system("pause");
		SDL_Quit();
		return -1;
	}

	//params: 
	//	window title, x and y pos, width, height, flags for creation
	SDL_Window* window = SDL_CreateWindow("My Awesome Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_SHOWN);//|SDL_WINDOW_FULLSCREEN

	if (window != NULL) {
		cout << "window created!" << endl;
	}
	else
	{
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	//create renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) {
		cout << "ERROR: " << SDL_GetError() << endl;
		system("pause");
		SDL_Quit(); //cleanup SDL memory
		return -1; //exit the program
	}

	//LOAD UP ASSETS TO DRAW
	//load image up as a surface. Surfaces are stored in RAM so we can access their pixel data from code
	//surfaces used for software rendering(booo, is not as fast as hardware)
	SDL_Surface* knightSurface = SDL_LoadBMP("assets/knight.bmp");
	//convert surface to texture. Textures are stored in gfx cards vram and used for Hardware rendering (yay!)
	//we cannot directly access the pixel data of a texture
	SDL_Texture* knightTexture = SDL_CreateTextureFromSurface(renderer, knightSurface);
	//dont need surface anymore, free memory up
	SDL_FreeSurface(knightSurface);

	//the region of the image that will be drawn
	SDL_Rect sourceRectangle;
	//get width and height from texture and store them into our rectangle
	//params: texture to query, format pointer, access pointer, width pointer, height pointer
	SDL_QueryTexture(knightTexture, NULL, NULL, &sourceRectangle.w, &sourceRectangle.h);
	sourceRectangle.x = 80;
	sourceRectangle.y = 80;
	sourceRectangle.w = 80;
	sourceRectangle.h = 80;

	//drawing the source rectangle region to fit into this rectangle
	//includes scaling up and down
	SDL_Rect destinationRectangle;
	destinationRectangle.x = 150;
	destinationRectangle.y = 230;
	destinationRectangle.w = sourceRectangle.w*4;
	destinationRectangle.h = sourceRectangle.h;

	//LOAD Run Sprite Sheet
	SDL_Texture* runSpriteSheet = IMG_LoadTexture(renderer, "assets/run.png");
	//this time source rect is copying 1 frame at a time
	SDL_Rect runClipRect = { 0,0,32,32 }; //x:0, y:0, w:32, h: 32
	SDL_Rect runDestRect = { 0,0, 128, 128 };

	//LOAD run surface up so we can colour key
	SDL_Surface* runSurface = IMG_Load("assets/run.png");
	//set colour key( this colour goes fully transparent)
	//params: surface, set or unset(1/0), uint for colour (using mapRGB to find index of this colour in image)
	SDL_SetColorKey(runSurface, 1, SDL_MapRGB(runSurface->format, 128, 128, 255));
	//turn into texture
	SDL_Texture* runSpriteSheetWithNoBG = SDL_CreateTextureFromSurface(renderer, runSurface);
	//clean up surface
	SDL_FreeSurface(runSurface);


	Animation anim1(runSpriteSheet, renderer, 4, 32, 32, 0.2);//0.2 = 200millisecond frame duration
	Animation anim2(runSpriteSheetWithNoBG, renderer, 4, 32, 32, 0.8);
	Animation anim3(runSpriteSheet, renderer, 4, 32, 32, 0.05);

	//SETUP INITIAL GAME OBJECTS
	list<GameObject*> gameObjects;
	//give this list reference to the game objects static variable
	GameObject::gameObjects = &gameObjects;

	//build a hero
	Hero* hero = new Hero();
	hero->animation = &anim2;
	hero->pos.x = 200;
	hero->pos.y = 200;
	hero->velocity.x = 200;
	hero->renderer = renderer;
	//add to the list
	gameObjects.push_back(hero);

	Wall* wall1 = new Wall();
	wall1->renderer = renderer;
	wall1->pos.x = 0;
	wall1->pos.y = 400;
	wall1->w = 200;
	wall1->h = 32;
	//add to the list
	gameObjects.push_back(wall1);

	Wall* wall2 = new Wall();
	wall2->renderer = renderer;
	wall2->pos.x = 0;
	wall2->pos.y = 0;
	wall2->w = 200;
	wall2->h = 32;
	//add to the list
	gameObjects.push_back(wall2);

	Ball* ball1 = new Ball();
	ball1->renderer = renderer;
	ball1->pos.x = 640;
	ball1->pos.y = 0;
	ball1->velocity.x = 200;
	gameObjects.push_back(ball1);


	//INPUT HANDLERS
	KeyboardHandler keyboardHandler;
	keyboardHandler.hero = hero;

	MouseHandler mouseHandler;
	mouseHandler.hero = hero;

	GameControllerHandler gameControllerHandler;
	gameControllerHandler.hero = hero;

	//Load up FONT file
	TTF_Font* font1 = TTF_OpenFont("assets/vermin_vibes_1989.ttf", 32);//params: file path, font size
	SDL_Color textColour = { 123, 0, 34, 0 };
	//create surface(image in ram) using font, colour and text
	SDL_Surface* textSurface = TTF_RenderText_Blended(font1, "Hello Game!", textColour);
	//convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	//cleanup surface
	SDL_FreeSurface(textSurface);

	//Text destination
	SDL_Rect textDest;
	textDest.x = 50;
	textDest.y = 50;
	//query texture for width and height
	SDL_QueryTexture(textTexture, NULL, NULL, &textDest.w, &textDest.h);

	//set last update to current number of milliseconds since start of game
	Uint32 lastUpdate = SDL_GetTicks();

	bool loop = true;
	//GAME LOOP
	while (loop) {
		//TIME MANAGEMENT STUFF
		//difference of current time minus the last update time
		Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
		//convert timeDiff to deltaTime (timeDiff as a fraction of a second)
		float DT = timeDiff / 1000.0;//e.g timeDiff = 200ms, then 200/1000 = 0.2 for DT
		//we want to measure timeDiff between gameLoop iterations, so we need to update lastUpdate
		lastUpdate = SDL_GetTicks();

		//set render draw colour with RGBA value (values between 0-255)
		SDL_SetRenderDrawColor(renderer, 255, 0, 168, 255);
		//clear the screen with current draw colour
		SDL_RenderClear(renderer);

		//change render draw colour
		SDL_SetRenderDrawColor(renderer, 0, 228, 255, 255);
		//make a rectangle to draw to the screen (x,y,w,h)
		SDL_Rect rect = { 100, 50, 300, 240 };

		//draw rect to screen with current draw colour
		SDL_RenderFillRect(renderer, &rect);

		//draw knight image
		SDL_RenderCopy(renderer, knightTexture, &sourceRectangle, &destinationRectangle);

		//render glob to screen
		SDL_RenderCopy(renderer, runSpriteSheet, &runClipRect, &runDestRect);
		int numberOfFrames = 4;
		//quick animation algorithm. each frame runs for 100ms on the screen
		runClipRect.x = runClipRect.w*((SDL_GetTicks() / 100) % numberOfFrames);

		//ANIMATIONS
		/*anim1.update(DT);
		anim2.update(DT);
		anim3.update(DT);
		anim1.draw(50, 50);
		anim2.draw(50, 100, 4.0f);
		anim3.draw(50, 250, true);*/

		//INPUT HANDLING
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			//check if user clicked close on window
			if (e.type == SDL_QUIT) {
				loop = false;
			}
			//check if user pressed a button
			if (e.type == SDL_KEYDOWN) {
				//see if ESC key pressed
				if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
					loop = false;
				}
			}

			keyboardHandler.update(&e);
			mouseHandler.update(&e);
			gameControllerHandler.update(&e);
		}

		if (gameControllerHandler.controller == NULL)
			keyboardHandler.updateHeld();
		else
			gameControllerHandler.updateHeld();

		//UPDATE AND DRAW ALL GAME OBJECTS
		for each (GameObject* gObj in gameObjects)
		{
			gObj->update(DT);
			gObj->draw();
		}

		//update textTexture
		string text = "Velocity.y = " + to_string(ball1->velocity.y);
		renderText(font1, textColour, text, renderer, textTexture);

		//drawing text on top of everything else
		SDL_QueryTexture(textTexture, NULL, NULL, &textDest.w, &textDest.h);
		SDL_RenderCopy(renderer, textTexture, NULL, &textDest);

		//swap the buffers, show our current frame we've been drawing
		SDL_RenderPresent(renderer);

		////sdl_getTicks returns number of milliseconds since start of program running
		//if (SDL_GetTicks() > 5000)
		//{
		//	loop = false;
		//}
	}
	//wait for 5 seconds (5000ms)
	//SDL_Delay(5000);

	//CLEANUP
	TTF_CloseFont(font1);
	SDL_DestroyTexture(textTexture);
	SDL_DestroyTexture(knightTexture);
	SDL_DestroyTexture(runSpriteSheet);
	SDL_DestroyTexture(runSpriteSheetWithNoBG);

	gameControllerHandler.close();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();



	system("pause");
	return 0;
}