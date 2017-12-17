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

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
/*************************************************************************
Constructors and Deconstructors
**************************************************************************/

/*************************************************************************
Functions
**************************************************************************/
//main: entry point of program
int main(int argc, char * argv[])
{
	//Use OutputDebugString for visual studio Output tab...
	std::stringstream ss;	//for Output messages
	OutputDebugString("Testing, 1 2 Tree! \n");

	//The window to render to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initializing SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		//Error
		ss << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
		OutputDebugString(ss.str().c_str());
		ss.clear();
		return -1;
	}
	else {
		//Create the window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			//error
			ss << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
			OutputDebugString(ss.str().c_str());
			ss.clear();
			return -2;
		}
		else {
			//Get the window surface
			screenSurface = SDL_GetWindowSurface(window);

			//Fill the surface white
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

			//Update the surface
			SDL_UpdateWindowSurface(window);

			//Wait two seconds
			SDL_Delay(2000);
		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}
