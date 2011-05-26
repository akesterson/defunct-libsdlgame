#include "Common.h"
#include <iostream>

SharedCanvas2D::SharedCanvas2D()
{
    this->canvas = NULL;
}

SharedCanvas2D::~SharedCanvas2D()
{
    this->releaseCanvas();
}

/**
 * @brief Return a pointer to the SDL_Surface pointer held by this shared canvas
 * @return SDL_Surface *
 */
SDL_Surface *SharedCanvas2D::getCanvas()
{
    return this->canvas;
}

/**
 * @brief Set the canvas pointer to the given object, and increment its refcount
 * @param obj A SharedCanvas2D object whose surface you want to share
 * @return 0 on success, 1 on failure
 */
int SharedCanvas2D::shareCanvas(SharedCanvas2D *obj)
{
    if ( obj == NULL || this->canvas != NULL || obj->getCanvas() == NULL ) {
	std::cerr << "Unable to share canvas from object " << obj << "\n";
	return 1;
    }
    this->canvas = obj->getCanvas();
    this->canvas->refcount += 1;
    return 0;
}

/**
 * @brief Set the canvas pointer to the given object, and increment its refcount
 * @param obj A SDL_Surface pointer who you want to share
 * @return 0 on success, 1 on failure
 */
int SharedCanvas2D::shareCanvas(SDL_Surface *obj)
{
  if ( obj == NULL ) {
    return 1;
  }

  this->canvas = obj;
  obj->refcount += 1;
  return 0;
}

/**
 * @fn SharedCanvas2D::releaseCanvas
 * @brief Decrement the refcount of the canvas we're currently sharing, and release it
 * @return 0 on success, 1 on failure
 */
int SharedCanvas2D::releaseCanvas()
{
    if ( this->canvas == NULL ) {
	return 0;
    }
    if ( this->canvas->refcount >= 2 ) {
	this->canvas->refcount -= 1;
	this->canvas = NULL;
	return 1;
    } else if ( this->canvas->refcount == 1 ) {
	SDL_FreeSurface(this->canvas);
	return 1;
    }
    // do nothing if canvas refcount == 0 (thou shalt not double free)
    return 0;
}
