/*************************************************************************
File name:		ltexture.h
Description:	Contains SDL texture wrapper class
**************************************************************************/

//#include guard.  Prevents double definitions
#ifndef LTEXTURE_H
#define LTEXTURE_H

//includes
#include <SDL.h>
#include <string>

//declarations

//Class definition
class LTexture {
public:
	LTexture();	// default constructor
	~LTexture();	//desctructor

	bool LoadFromFile(std::string path, SDL_Renderer* renderer);	//Loads image at specified path
	void Free();	//Deallocates surface
	void Render(int x, int y, SDL_Renderer* renderer);	//Renders texture at given point
	
	//Gets image at dimensions
	int GetWidth();
	int GetHeight();

protected:
private:
	SDL_Texture* _texture;	//underlying hardware texture

	//Image dimensions
	int _iWidth;
	int _iHeight;
};
#endif	//#ifndef LTEXTURE_H