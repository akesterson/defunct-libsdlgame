#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <vector>
#include <SDL.h>
#include "Renderable.h"
#include "Animation.h"

#define STATE_NONE		0
#define STATE_DYING		2
#define STATE_DEAD		4
#define STATE_MOVEXAXIS		(8 | 16) 
#define STATE_MOVELEFT		8
#define STATE_MOVERIGHT		16
#define STATE_MOVEYAXIS		(32 | 64)
#define STATE_MOVEUP		32
#define STATE_MOVEDOWN		64
#define STATE_VISIBLE		128
#define STATE_INVINCIBLE	256
#define STATE_SPAWNWAIT		512
#define STATE_FADEIN		1024
#define STATE_FADEOUT		2048
#define STATE_ATTACKING		4096
#define STATE_MOVEIN		8192
#define STATE_MOVEOUT		16384
#define STATE_RENDERPARENT	32768 	// this status flag specifies that, when an Actor has a Parent,
					// the position returned by this actor should be relative to their parent

#define STATE_DEFAULT		0xFFFFFFFF

class Actor : public AnimatedRenderable
{
protected:
    int state;
    FrameCounter logicCounter;
    unsigned int userType;
public:
    AnimationStateMap *animMap;
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

    Actor *parent;
};

typedef std::vector<Actor *> ActorList;
typedef std::vector<Actor *>::iterator ActorListIterator;

#endif // __ACTOR_H__
