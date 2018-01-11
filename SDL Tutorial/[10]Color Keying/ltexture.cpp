/*************************************************************************
File name:		ltexture.cpp
Description:	definitions for SDL texture wrapper class
**************************************************************************/

/*************************************************************************
Includes
**************************************************************************/
#include "ltexture.h"
#include <SDL_image.h>

/*************************************************************************
Definitions
**************************************************************************/
/*************************************************************************
Constructors and Deconstructors
**************************************************************************/
LTexture::LTexture()
{
	//Initialize
	this->_texture = NULL;
	this->_iWidth = 0;
	this->_iHeight = 0;

}

LTexture::~LTexture()
{
	//Get rid of preexisting textures
	this->Free();
}
/*************************************************************************
Functions
**************************************************************************/
bool LTexture::LoadFromFile(std::string path, SDL_Renderer* renderer)
{
	this->Free();

	SDL_Texture* newTexture = NULL;	//The final texture

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
	}
	else {
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) {
		}
		else {
			//Get image dimension
			this->_iWidth = loadedSurface->w;
			this->_iHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	this->_texture = newTexture;

	return _texture != NULL;
}

void LTexture::Free()
{
	//Free texture if it exists and reinitialize some variables
	if (this->_texture != NULL) {
		SDL_DestroyTexture(this->_texture);
		this->_texture = NULL;
		this->_iWidth = _iHeight = 0;
	}
}

void LTexture::Render(int x, int y, SDL_Renderer* renderer)
{
	//Set rendering space to render to screnn
	SDL_Rect renderQuad = { x,y,_iWidth,_iHeight };
	SDL_RenderCopy(renderer, this->_texture, NULL, &renderQuad);
}

int LTexture::GetWidth()
{
	return this->_iWidth;
}

int LTexture::GetHeight()
{
	return this->_iHeight;
}