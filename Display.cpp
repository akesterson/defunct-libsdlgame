#include "Display.h"
#include "Actor.h"
#include <iostream>

Display::Display()
{
    this->active = 0;
    return;
}

Display::~Display()
{
    if ( this->canvas != NULL && this->canvas->refcount > 0 ) {
	SDL_FreeSurface(this->canvas);
    }
}

int Display::isActive()
{
    return this->active;
}

void Display::setActive(int active)
{
    this->active = active;
}

int Display::initVideo(Vector pos, int w, int h, int depth, int flags)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    int rmask = 0xff000000;
    int gmask = 0x00ff0000;
    int bmask = 0x0000ff00;
  //int amask = 0x000000ff;
#else
    int rmask = 0x000000ff;
    int gmask = 0x0000ff00;
    int bmask = 0x00ff0000;
  //int amask = 0xff000000;
#endif
    this->setPosition(pos);
    this->canvas = SDL_CreateRGBSurface(flags, w, h, depth, rmask, gmask, bmask, 0);
    if ( this->canvas == NULL ) {
	//std::cerr << "Failed to create new SDL surface. Error was: " << SDL_GetError() << "\n";
	return 1;
    }
    ////std::cerr << "Created new SDL surface at " << this->canvas << "\n";
    return 0;
}

int Display::addActor(Actor *actor)
{
     if ( actor == NULL ) {
         //std::cerr << "NULL actor!";
         return 1;
     }
     //std::cerr << "Adding actor " << actor << " to Display " << this << "\n";
     ActorListIterator iter = this->actors.begin();
     while ( iter != this->actors.end() ) {
	 if ( *iter == actor ) {
	     return 1;
	 }
	 iter++;
     }
     
     this->actors.push_back(actor);
     return 0;
}

Actor *Display::getActor(unsigned int index)
{
    if ( index < this->actors.size() ) {
	return this->actors.at(index);
    }
    return NULL;
}

int Display::removeActor(Actor *actor)
{
    ActorListIterator iter = this->actors.begin();
    //std::cerr << "Trying to delete actor " << actor << " from master list\n";
    while ( iter != this->actors.end() ) {
        if ( (*iter) == actor ) {
	  //std::cerr << "Found it!\n";
            this->actors.erase(iter);
            return 0;
        }
        iter++;
    }
    return 1;
}

SDL_Surface *Display::nextFrame()
{
    //std::cerr << "[" << this << "]::nextFrame active = " << this->active << "\n";
    if ( this->active == 1 ) {
	return this->canvas;
    } else {
	return NULL;
    }
}

SDL_PixelFormat *Display::pixelFormat()
{
    return this->canvas->format;
}

int Display::objectsOfType(unsigned int userType)
{
    int ocount = 0;
    ActorListIterator iter = this->actors.begin();
    while ( iter != this->actors.end() ) {
        if ( (*iter)->getUserType() == userType ) {
	    ocount += 1;
	}
	iter++;
    }
    return ocount;
}
