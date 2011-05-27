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

/**
 * @return integer, 0 for inactive, 1 for active
 * @brief return the active state of this display
 */

int Display::isActive()
{
    return this->active;
}

/**
 * @param active integer, 0 or 1
 * @brief Toggle the active flag on this display
 */

void Display::setActive(int active)
{
    this->active = active;
}

/**
 * @param pos Vector, position of this window relative to any parents
 * @param w integer, width of the canvas
 * @param h integer, height of the canvas
 * @param depth integer, bit depth of the canvas
 * @param flags integer, flags passed to SDL_CreateRGBSurface
 * @brief Setup the rendering surface this display will use
 * @return 0 on success, 1 on failure
 */

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

/**
 * @param actor Actor *, the actor to add to the actor list
 * @brief Add 'actor' to this display
 * @return 0 on success, 1 on failure
 *
 * Adds an actor to this display's actor list to be managed on the canvas. 
 * Actors are pushed to the back end of the list by default.
 */
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

/**
 * @param index unsigned integer
 * @brief return the actor at index 'index' of this display's actor list
 * @return (Actor *), actor if present in the list, or NULL otherwise
 */

Actor *Display::getActor(unsigned int index)
{
    if ( index < this->actors.size() ) {
	return this->actors.at(index);
    }
    return NULL;
}

/**
 * @param actor (Actor *), actor to remove from this display
 * @return 0 on success, 1 on failure
 * @brief Remove a given actor from this display's actor list
 */
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

/**
 * @return SDL_PixelFormat
 * @brief return the pixel format of the current canvas
 */
SDL_PixelFormat *Display::pixelFormat()
{
    return this->canvas->format;
}

/**
 * @param userType (unsigned int) the user type to check for
 * @brief Returns a count of all objects in this display's actor list that match the given user defined type
 */ 
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
