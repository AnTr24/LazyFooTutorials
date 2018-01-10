/*************************************************************************
File name:		main.cpp
Description:	The entry point of the program.
C++ Tutorial by lazyfoo.net
**************************************************************************/

/*************************************************************************
Includes
**************************************************************************/
#include <SDL.h>
#include <stdio.h>
#include <windows.h>	//for OutputDebugString
#include <sstream>
#include <SDL_image.h>

/*************************************************************************
Prototypes and Declarations
**************************************************************************/
bool Init();								//Starts up SDL and creates a window
bool LoadMedia();							//Loads media
void Close();								//Frees media and shuts down SDL
void OutputMessage();						//Debugging function, Write to Output window
SDL_Texture* LoadTexture(std::string path);	//Loads individual image as texture

SDL_Window* gWindow = NULL;			//Window to render to
SDL_Renderer* gRenderer = NULL;		//Window renderer
SDL_Texture* gTexture = NULL;		//Currently displayed texture

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
		OutputMessage();
		Close();
		return -1;
	}

	//Load media
	if (!LoadMedia()) {
		ss << "Failed to load media!\n";
		OutputMessage();
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

		//Top left corner viewport
		SDL_Rect topLeftVP;
		topLeftVP.x = 0;
		topLeftVP.y = 0;
		topLeftVP.w = SCREEN_WIDTH / 2;
		topLeftVP.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(gRenderer, &topLeftVP);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//Top right viewport
		SDL_Rect topRightVP;
		topRightVP.x = SCREEN_WIDTH / 2;
		topRightVP.y = 0;
		topRightVP.w = SCREEN_WIDTH / 2;
		topRightVP.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(gRenderer, &topRightVP);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

		//Bottom viewport
		SDL_Rect bottomVP;
		bottomVP.x = 0;
		bottomVP.y = SCREEN_HEIGHT / 2;
		bottomVP.w = SCREEN_WIDTH;
		bottomVP.h = SCREEN_HEIGHT / 2;
		SDL_RenderSetViewport(gRenderer, &bottomVP);

		//Render texture to screen
		SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

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
	OutputMessage();

	//Initialzie SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		ss << "SDL could not initialize! SDL_Error: %s\n" << SDL_GetError() << "\n";
		OutputMessage();
		success = false;
	}
	else {
		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			ss << "Window could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
			OutputMessage();
			success = false;
		}
		else {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				//failed to create renderer
				ss << "Renderer could not be created! SDL_Error: %s\n" << SDL_GetError() << "\n";
				OutputMessage();
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
					OutputMessage();
					success = false;
				}
			}
		}
	}

	return success;
}

//bool LoadMedia()
//Attempts to load texture
//return bool : true if texture successfully loaded, false if an error occured
bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	gTexture = LoadTexture("img/viewport.png");
	if (gTexture == NULL)
	{
		ss << "Failed to load texture image!";
		OutputMessage();
		success = false;
	}
	

	return success;
}

//SDL_Texture* LoadTexture(std::string path)
//Loads texture at specified path
//std::string path : specified path
SDL_Texture* LoadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		ss << "Failed to load default image!\n SDL_image Error: " << IMG_GetError();
		OutputMessage();
	}
	else {
		//Loaded image, now create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL) {
			ss << "Unable to create texture from " << path << "!\n SDL_image Error: " << SDL_GetError();
			OutputMessage();
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


//void Close() 
//Frees resources and closes SDL
void Close()
{
	ss << "Shutting down...\n";
	OutputMessage();

	//Deallocate surfaces and textures
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	//Destroy window
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);	//also frees up the realted window surface(s)
	gRenderer = NULL;
	gWindow = NULL;

	//Quit SDL subsystems;
	IMG_Quit();
	SDL_Quit();
}

//void OutputMessage()
//Outputs the stringstream ss to the Output tab
//mainly for debugging
//Side effect: clears ss contents after outputting.
void OutputMessage()
{
	OutputDebugString(ss.str().c_str());
	ss.str("");
	ss.clear();
}
