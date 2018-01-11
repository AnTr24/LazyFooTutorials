/*************************************************************************
File name:		main.cpp
Description:	The entry point of the program.
C++ Tutorial by lazyfoo.net
**************************************************************************/

/*************************************************************************
Includes
**************************************************************************/
#include "ltexture.h"
#include "debug.h"

#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>

/*************************************************************************
Prototypes and Declarations
**************************************************************************/
bool Init();								//Starts up SDL and creates a window
bool LoadMedia();							//Loads media
void Close();								//Frees media and shuts down SDL

SDL_Window* gWindow = NULL;			//Window to render to
SDL_Renderer* gRenderer = NULL;		//Window renderer

//Scene textures
LTexture* ltFoo = NULL;			
LTexture* ltBackground = NULL;	

const int SCREEN_WIDTH = 640;		//width of screen
const int SCREEN_HEIGHT = 480;		//height of screen
std::stringstream ss;				//for debugging; Output messages

/*************************************************************************
Constructors and Deconstructors
**************************************************************************/

/*************************************************************************
Functions
**************************************************************************/
//main: entry point of program
int main(int argc, char * argv[])
{
	//Main loop flag
	bool quit = false;

	//Event handler 
	SDL_Event e;

	//Startup SDL and create window
	if (!Init()) {
		ss << "Failed to initialize!\n";
		OutputMessage(ss);
		Close();
		return -1;
	}

	//Load media
	if (!LoadMedia()) {
		ss << "Failed to load media!\n";
		OutputMessage(ss);
		Close();
		return -2;
	}

	//main application loop
	while (!quit)
	{
		//Handle events on a queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT) {
				quit = true;
			}

		}//end of event queue

		//Clear screen
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);


		//Render here
		ltBackground->Render(0, 0, gRenderer);	//Background gets rendered first
		ltFoo->Render(240,190, gRenderer);

		//Update the screen
		SDL_RenderPresent(gRenderer);

	}//end of application loop
	//Free resources and close SDL
	Close();

	return 0;
}

bool Init()
{
	//Initialization flag
	bool success = true;

	ss << "Initializing...\n";
	OutputMessage(ss);

	//Initialzie SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ss << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << "\n";
		OutputMessage(ss);
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			ss << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
			OutputMessage(ss);
			success = false;
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				//failed to create renderer
				ss << "Renderer could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
				OutputMessage(ss);
				success = false;
			}
			else {
				//Renderer initialized
				//initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					ss << "SDL_image could not initialize! SDL_image Error: %s\n" << IMG_GetError() << "\n";
					OutputMessage(ss);
					success = false;
				}
			}
		}
	}

	ltFoo = new LTexture();
	ltBackground = new LTexture();

	return success;
}

//bool LoadMedia()
//Attempts to load texture
//return bool : true if texture successfully loaded, false if an error occured
bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Foo' texture
	if (!ltFoo->LoadFromFile("img/foo.png",gRenderer)) {
		ss << "Failed to load Foo' texture image!\n";
		OutputMessage(ss);
		success = false;
	}

	//Load Background' texture
	if (!ltBackground->LoadFromFile("img/background.png", gRenderer)) {
		ss << "Failed to load Background' texture image!\n";
		OutputMessage(ss);
		success = false;
	}

	return success;
}

//void Close() 
//Frees resources and closes SDL
void Close()
{
	ss << "Shutting down...\n";
	OutputMessage(ss);

	//Deallocate surfaces and textures
	ltFoo->Free();
	ltBackground->Free();

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);	//also frees up the realted window surface(s)
	gRenderer = NULL;
	gWindow = NULL;

	//Quit SDL subsystems;
	IMG_Quit();
	SDL_Quit();
}
