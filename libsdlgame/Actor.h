#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>
#include <SDL.h>
#include "Renderable.h"
#include "Animation.h"


#define STATE_NONE		0 /**< No state */
#define STATE_DYING		2 /**< Dying */
#define STATE_DEAD		4 /**< Dead */
#define STATE_MOVEXAXIS		(8 | 16) /**< Moving on X axis */
#define STATE_MOVELEFT		8 /**< Moving to the left */
#define STATE_MOVERIGHT		16 /**< moving to the right */
#define STATE_MOVEYAXIS		(32 | 64) /** Moving on the Y axis */
#define STATE_MOVEUP		32 /**< Moving up */
#define STATE_MOVEDOWN		64 /**< Moving down */
#define STATE_VISIBLE		128 /**< Visible */
#define STATE_INVINCIBLE	256 /**< Invincible */
#define STATE_SPAWNWAIT		512 /**< Waiting for respawn */
#define STATE_FADEIN		1024 /**< Fading in */
#define STATE_FADEOUT		2048 /**< Fading out */
#define STATE_ATTACKING		4096 /**< Attacking */
#define STATE_MOVEIN		8192 /**< I don't know */
#define STATE_MOVEOUT		16384 /**< I don't know */
#define STATE_RENDERPARENT	32768 /**< When an Actor has a Parent, the position returned by this actor should be relative to their parent */

#define STATE_DEFAULT		0xFFFFFFFF /**< Default state is ZOMGEVERYFUCKINTHING */

/**
 * @class Actor
 * @brief A class that represents an on-screen object that moves, has logic, etc
 */

class Actor : public AnimatedRenderable
{
protected:
  int state; /**< A bitmask of the various STATE_* defines to define actor's current state */
  FrameCounter logicCounter; /**< A FrameCounter used to track logic timing */
  unsigned int userType; /**< A custom integer value that can be set by users to define custom types to track */
public:
  AnimationStateMap *animMap; /**< Map of Animation objects keyed by state value */
  Actor(AnimationStateMap *animMap, Vector position, Vector velocity);
  Actor();
  virtual ~Actor();
  virtual SDL_Surface *nextFrame();
  virtual Vector getPosition(char gfxanchor = 0);
  virtual int addAnimation(Animation *anim, unsigned int key);
  virtual Animation *getAnimation(unsigned int stateKey);
  virtual void setState(int state);
  virtual int getState();
  virtual void addState(int state);
  virtual void removeState(int state);
  virtual int hasState(int state);
  virtual void update();
  virtual void collide(Actor *ptr) {};
  virtual unsigned int getUserType();
  virtual void setUserType(unsigned int type);
  
  Actor *parent; /**< Can be used to specify a parent Actor, relative to whom this Actor will be rendered. */
};


typedef std::vector<Actor *> ActorList;
typedef std::vector<Actor *>::iterator ActorListIterator;

#endif // __ACTOR_H__
