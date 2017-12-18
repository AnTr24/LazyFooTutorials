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

/*************************************************************************
Prototypes and Declarations
**************************************************************************/
bool Init();								//Starts up SDL and creates a window
bool LoadMedia();							//Loads media
SDL_Surface* LoadSurface(std::string path);	//Loads individual image
void Close();								//Frees media and shuts down SDL
void OutputMessage();						//Debugging function, Write to Output window

std::stringstream ss;				//for debugging; Output messages
SDL_Window* gWindow = NULL;			//Window to render to
SDL_Surface* gScreenSurface = NULL;	//Surface contained by window
SDL_Surface* gImage = NULL;			//Image to load and show on screen
const int SCREEN_WIDTH = 640;		//width of screen
const int SCREEN_HEIGHT = 480;		//height of screen

//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

//The images that correspond to a  keypress
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

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

	//Set current surface to default
	gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

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
			else if (e.type == SDL_KEYDOWN) {
				//Select surfaces based on key press
				switch (e.key.keysym.sym) 
				{
					case SDLK_UP:
						gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
						break;
					case SDLK_DOWN:
						gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
						break;
					case SDLK_LEFT:
						gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
						break;
					case SDLK_RIGHT:
						gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
						break;
					default:
						gImage = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						break;
				}
			}


		}//end of event queue

		 //Apply image
		SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);

		//Update the surface
		SDL_UpdateWindowSurface(gWindow);

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
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}

	return success;
}

//bool LoadMedia()
//Attempts to load all BMP files
//return bool : true if bmp(s) successfully loaded, false if an error occured
bool LoadMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("img/press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		ss << "Failed to load default image!\n";
		OutputMessage();
	}

	//Load up surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = LoadSurface("img/up.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		ss << "Failed to load up image!\n";
		OutputMessage();
	}

	//Load down surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = LoadSurface("img/down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		ss << "Failed to load default down!\n";
		OutputMessage();
	}

	//Load left surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = LoadSurface("img/left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		ss << "Failed to load default left!\n";
		OutputMessage();
	}

	//Load right surface
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("img/right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		ss << "Failed to load right image!\n";
		OutputMessage();
	}

	return success;
}

//SDL_Surface* LoadSurface(std::string path)
//Loads image at specified path
//std::string path : specified path
SDL_Surface* LoadSurface(std::string path)
{
	SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
	if (loadedSurface == NULL)
	{
		ss << "Unable to load image! SDL_Error: \n" << SDL_GetError() << "\n";
		OutputMessage();
	}

	return loadedSurface;
}


//void Close() 
//Frees resources and closes SDL
void Close()
{
	ss << "Shutting down...\n";
	OutputMessage();

	//Deallocate surface
	SDL_FreeSurface(gImage);
	gImage = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);	//also frees up the realted window surface(s)
	gScreenSurface = NULL;
	gWindow = NULL;

	//Quit SDL subsystems;
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
