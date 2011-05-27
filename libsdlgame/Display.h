#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "Actor.h"
#include "Renderable.h"
#include "FontRenderer.h"
#include <SDL.h>

/** 
 * @class Display
 * @brief A class for managing a scene
 *
 * This class provides a method of managing scenes. It gives access
 * to a rendering port through Renderable/SharedCanvas2D; it provides
 * management of on-screen actors, and provides a basic logic update
 * framework.
 */

class Display : public Renderable, public SharedCanvas2D
{
protected:
  ActorList actors; /**< list of actors currently assigned to this display */
  int active; /**< boolean flag, 0 for inactive, 1 for active. This controls visibility and logic. */
public:
  Display();
  virtual ~Display();
  virtual int isActive();
  virtual void setActive(int active);
  virtual int initVideo(Vector pos, int w, int h, int depth, int flags);
  virtual SDL_Surface *nextFrame();
  virtual int addActor(Actor *actor = NULL);
  virtual Actor *getActor(unsigned int index);
  virtual int removeActor(Actor *actor);
  virtual void update(int logicOnly = 0) {};
  SDL_PixelFormat *pixelFormat();
  virtual int objectsOfType(unsigned int userType);
};

#endif // __DISPLAY_H__
