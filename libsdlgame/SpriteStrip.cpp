#include "Renderable.h"
#include "SpriteStrip.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SpriteStrip::SpriteStrip()
{
    return;
}

SpriteStrip::~SpriteStrip()
{
    this->clearFrames();
}

int SpriteStrip::loadFromFile(std::string filename, int f_w, int f_h, Vector colorkey)
{
    SDL_Surface *imgsurf = NULL;
    imgsurf = IMG_Load(filename.c_str());
    if ( imgsurf != NULL ) {
      //std::cerr << "Splitting frames from " << filename.c_str() << " (" << imgsurf->w << "x" << imgsurf->h << "...\n";
	return this->loadFromSurface(imgsurf, f_w, f_h, colorkey);
    } else {
      //std::cerr << "Couldn't load image from file!\n";
	return 1;
    }
}

// pass 0 in f_w and f_h to return a single frame into the strip that's the size of the image
// pass 0 in just one of them to indicate that each sprite is the entire width or height of 
// image
int SpriteStrip::loadFromSurface(SDL_Surface *image, int f_w, int f_h, Vector colorkey)
{
    int stripping = 1;
    SDL_Surface *newSurf = NULL;
    SDL_Rect copyRect = {0, 0, f_w, f_h };

    if ( image == NULL )
	return 1;

    if ( f_w == 0 ) {
	f_w = image->w;
	copyRect.w = f_w;
    }
    if ( f_h == 0 ) {
	f_h = image->h;
	copyRect.h = f_h;
    }

    while ( stripping == 1) {
	newSurf = SDL_CreateRGBSurface(image->flags, f_w, f_h, image->format->BitsPerPixel, 0, 0, 0, 0);
	if ( newSurf != NULL ) {
	    if ( colorkey.x > -1 && colorkey.y > -1 && colorkey.z > -1 ) {
		////std::cerr << "Setting color key\n";
		SDL_SetColorKey(newSurf, SDL_SRCCOLORKEY, 
			SDL_MapRGB(newSurf->format, 
				(Uint8) colorkey.x, 
				(Uint8) colorkey.y, 
				(Uint8) colorkey.z));
	    }
	    //std::cout << "Blitting new surface (" << copyRect.w << "x" << copyRect.h << " (src : " << copyRect.x << "," << copyRect.y << ")\n";
	    if ( SDL_BlitSurface(image, &copyRect, newSurf, NULL) == 0 ) {
		this->frames.push_back(newSurf);
		newSurf = NULL;
	    } else {
                //std::cerr << "Unable to blit from sprite sheet to sprite element.\n";
		exit(1);
	    }
	    if ( copyRect.x + (f_w*2) > image->w ) {
		copyRect.x = 0;
		copyRect.y += f_h;
		// overflowing image?
		if ( copyRect.y >= image->h ){
		    stripping = 0;
		    continue;
		}
	    } else {
		copyRect.x += f_w;
	    }
	} else {
	    //std::cerr << "Unable to allocate memory for sprite strip element.\n";
	    exit(1);
	}
    }
    //std::cout << "Split " << this->frames.size() << " frames into new sprite strip\n";
    return 0;
}

SDL_Surface *SpriteStrip::getFrame(unsigned int frame)
{
    if ( frame < this->frames.size() ) {
	return this->frames.at(frame);
    } else {
	////std::cerr << "Attempt to access beyond end of sprite strip (" << frame << " of " << this->frames.size() << ") on strip " << this << "\n";
	return NULL;
    }
}

int SpriteStrip::width()
{
    if ( this->frames.size() > 0 ) {
	return this->frames.at(0)->w;
    }
    return 0;
}

int SpriteStrip::height()
{
    if ( this->frames.size() > 0 ) {
	return this->frames.at(0)->h;
    }
    return 0;
}

void SpriteStrip::clearFrames()
{
    std::vector<SDL_Surface *>::iterator iter;
    while ( this->frames.size() > 0 ) {
	iter = this->frames.end();
	if ( *iter != NULL ) {
	    SDL_FreeSurface(*iter);
	    this->frames.erase(iter);
	}
    }
}

int SpriteStrip::numFrames()
{
    if ( this == NULL ) {
	//std::cerr << "SpriteStrip::numFrames got passed a NULL this-> pointer. How the FUCK did that happen?!\n";
	return 0;
    }
    return this->frames.size();
}

SpriteStripIter SpriteStrip::iterator()
{
    return (SpriteStripIter)this->frames.begin();
}


