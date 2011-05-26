#ifndef __RENDERABLE_H__
#define __RENDERABLE_H__

#include <SDL.h>
#include "Common.h"

/**
 * @class Renderable
 * @brief A base class for objects that are capable of returning renderable data
 * 
 * This base class provides default mechanisms for an object to return renderable
 * data, track position and velocity.
 */
class Renderable
{
protected:
  Vector position; /**< The position of the renderable object */
  Vector velocity; /**< The velocity (if any) of the renderable object */
 public:
  Renderable();
  Vector getPosition();
  Vector getVelocity();
  void setPosition(Vector);
  void setVelocity(Vector);
  virtual SDL_Surface *nextFrame();
};

/**
 * @class AnimatedRenderable
 * @brief A subclass of Renderable for objects that are renderable and animated
 *
 * This subclass defines the interface for libsdlgame objects that are both
 * Renderable, but that also perform some kind of internal logic to return
 * different surfaces each call. 
 */
class AnimatedRenderable : public Renderable
{
 protected:
  FrameCounter frameCounter; /**< Current frame within the current animation */
 public:
  AnimatedRenderable();
  FrameCounter getFrameCounter();
  int setFrameCounter(FrameCounter fc);
};

#endif // __RENDERABLE_H__
