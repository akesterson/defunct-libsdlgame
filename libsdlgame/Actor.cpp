//#include "Game.h"
#include "Common.h"
#include "Actor.h"
#include "Animation.h"
#include <SDL.h>
#include <iostream>

Actor::Actor()
{
    this->userType = 0;
    this->state = STATE_NONE;
    this->animMap = new AnimationStateMap();
    this->position.x = 0;
    this->position.y = 0;
    this->position.z = 0;
    this->velocity.x = 0;
    this->velocity.y = 0;
    this->velocity.z = 0;
    this->parent = NULL;
    this->frameCounter.curFrame = 0;
    this->frameCounter.lastFrame = 0;
    this->frameCounter.lastTime = SDL_GetTicks();
    this->logicCounter.lastTime = SDL_GetTicks();
}

Actor::~Actor()
{
}

Actor::Actor(AnimationStateMap *animMap, Vector position, Vector velocity)
{
    this->animMap = animMap;
    if ( this->animMap == NULL ) {
	this->animMap = new AnimationStateMap();
    }
    this->position.x = position.x;
    this->position.y = position.y;
    this->position.z = position.z;
    this->velocity.x = velocity.x;
    this->velocity.y = velocity.y;
    this->velocity.z = velocity.z;

    this->frameCounter.curFrame = 0;
    this->frameCounter.lastFrame = 0;
    this->frameCounter.lastTime = SDL_GetTicks();

    this->logicCounter.lastTime = SDL_GetTicks();
    this->state = 0;
}

int Actor::addAnimation(Animation *anim, unsigned int stateKey)
{
    // I started letting this accept NULL sprite strips when STATE_DEFAULT was introduced
    // so that you could force an actor to be culled from the display list by setting its
    // STATE_DEAD to NULL manually. 
    if ( this->animMap == NULL) {
	//////std::cerr << "Pussying out.";
	return 1;
    }
    //////std::cerr << "Adding " << anim << " to animation map " << this->animMap << " for " << this << " at state " << stateKey << "\n";
    if ( this->animMap->count(stateKey) > 0 ) {
	this->animMap->erase(stateKey);
    }
    (*this->animMap)[stateKey] = anim;
    return 0;
}

Animation *Actor::getAnimation(unsigned int stateKey)
{
    //////std::cerr << "stateKey " << stateKey << " this->animMap " << this->animMap << " count " << this->animMap->count(stateKey) << "\n";
    if ( this->animMap == NULL ) {
	return NULL;
    }
    if ( this->animMap != NULL && this->animMap->count(stateKey) > 0  ) {
	//////std::cerr << "Returning SOMETHING...\n";
	return (*this->animMap)[stateKey];
    } else if ( this->animMap->count(STATE_DEFAULT) > 0 ) {
	return (*this->animMap)[STATE_DEFAULT];
    }

    return NULL;
}

SDL_Surface *Actor::nextFrame()
{
    SDL_Surface *newFrame = NULL;
    Animation *anim = this->getAnimation(this->state);
    if ( anim != NULL ) {
	newFrame = anim->nextFrame(0, 0, this);
	return newFrame;
    }
    ////std::cerr << "Got NULL animation!\n";
    return NULL;
}

void Actor::setState(int state)
{
    this->state = state;
}

void Actor::addState(int state)
{
    this->state = this->state | state;
    //////std::cerr << "State is now " << this->state << " for actor " << this << "\n";
}

void Actor::removeState(int state)
{
    if ( this->hasState(state) ) {
        this->state = this->state ^ state;
    }
    //////std::cerr << "State is now " << this->state << " for actor " << this << "\n";
}

Vector Actor::getPosition(char gfxanchor)
{
    Animation *anim = NULL;
    Vector pos;
    Vector parentPos;
    pos.x = this->position.x;
    pos.y = this->position.y;
    pos.z = this->position.z;
    if ( gfxanchor == 1 ) {
	anim = this->getAnimation(this->state);
    	if ( anim )
	    pos = anim->anchorAt(pos);
    }
    if ( this->parent != NULL && this->hasState(STATE_RENDERPARENT)) {
	parentPos = this->parent->getPosition();
	pos.x += parentPos.x;
	pos.y += parentPos.y;
	pos.z += parentPos.z;
     }
    return pos;
}

void Actor::update()
{
    Animation *curAnim;
    Vector parentPos;

    //if ( (SDL_GetTicks() - this->logicCounter.lastTime) >  (1000/GAMEFPS) ) {
	this->logicCounter.lastTime = SDL_GetTicks();

	if ( this->hasState(STATE_MOVEXAXIS)) {
	    this->position.x += this->velocity.x;
	} else {
	    if ( this->hasState(STATE_MOVERIGHT) )
		this->position.x += this->velocity.x;
	    if ( this->hasState(STATE_MOVELEFT) )
		this->position.x -= this->velocity.x;
	}
	if ( this->hasState(STATE_MOVEYAXIS) ) {
	    this->position.y += this->velocity.y;
	} else {
	    if ( this->hasState(STATE_MOVEDOWN) )
		this->position.y += this->velocity.y;
	    if ( this->hasState(STATE_MOVEUP) )
		this->position.y -= this->velocity.y;
	}
	if ( this->hasState(STATE_MOVEIN) )
	    this->position.z -= this->velocity.z;
        if ( this->hasState(STATE_MOVEOUT) )
	    this->position.z += this->velocity.z;
	//std::cerr << "x = " << this->position.x << " y = " << this->position.y << "\n"; 
	//std::cerr << " state = " << this->state << "\n";
	
	curAnim = this->getAnimation(this->state);
	//std::cerr << curAnim->numFrames();
	//if ( curAnim != NULL )
	    //std::cerr << "Dying object " << this << "has animation " << curAnim << " current frame " << this->frameCounter.curFrame << " of " << curAnim->numFrames() << " state " << this->state << "\n";
	//std::cerr.flush();
	if ( ((curAnim != NULL) && (this->frameCounter.curFrame+1 >= (unsigned int)curAnim->numFrames()) && (this->hasState(STATE_DYING))) ||
	   (curAnim == NULL && this->hasState(STATE_DYING)) ) {
	    // we're dead
	    //std::cerr << "Actor " << this << " is dead.\n";
	    this->state = STATE_DEAD;
	}
    //}
}

int Actor::hasState(int state)
{
    if ( (this->state & state) == state )
	return 1;
    else return 0;
}

int Actor::getState()
{
    return this->state;
}

unsigned int Actor::getUserType()
{
    return this->userType;
}

void Actor::setUserType(unsigned int type)
{
    this->userType = type;
}


