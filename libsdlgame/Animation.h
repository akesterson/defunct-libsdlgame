#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <SDL.h>
#include "SpriteStrip.h"
#include "Renderable.h"
#include <map>

/**
 * @class Animation
 * @brief A class for handling animation states related to sprite strips
 */

class Animation
{
 protected:
  unsigned int lastTime; /**< the time of the last animation update */
  SpriteStrip *strip; /**< The spritestrip used by this animation */
  unsigned int timeStep; /**< The number of ticks between each frame */
  int fps; /**< locked frames per second */
  int loop; /**< 1 = loop continually, 0 = play once and stop */
  Vector anchor; /**< Position relative to (0, 0) which is the actual anchor point for this animation */
 public:
  Animation();
  virtual ~Animation();
  /** @brief Returns the value of 'loop' */
  int getLoop() { return this->loop; };
  int setStrip(SpriteStrip *strip, int fps, int loop = 0, Vector anchor = (Vector){0,0,0});
  SDL_Surface *nextFrame(int curFrame, int lastTime, AnimatedRenderable *actorSource = NULL);
  int numFrames();
  Vector anchorAt(Vector position);
};

/** A hashmap to tie animation states (integer bitflags) to animation pointers */
typedef std::map<unsigned int, Animation *> AnimationStateMap;
/** An iterator for AnimationStateMap */
typedef std::map<unsigned int, Animation *>::iterator AnimationStateMapIterator;


#endif // __ANIMATION_H__
