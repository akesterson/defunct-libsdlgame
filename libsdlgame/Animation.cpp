#include <SDL.h>
#include "SpriteStrip.h"
#include "Animation.h"
#include "Actor.h"
#include <iostream>

/** TODO: Animation has mysterious segfault problems when it is used static; e.g.
 *
 *	Animation anim; <-- using this object will eventually cause a segfault thru the framecounters/indexers
 *	..
 *	Animation *anim = new Animation() <-- ... using this one will not
 */
Animation::Animation()
{
    this->strip = NULL;
    this->timeStep = 0;
    this->fps = 0;
    this->anchor.x = 0;
    this->anchor.y = 0;
}

Animation::~Animation()
{
}

/**
 * @brief Sets the sprite strip used by this animation
 * @param strip sprite strip from which animation frames should be pulled
 * @param fps lock the animation speed to a given frames per second
 * @param loop 0 = play once and stop, 1 = loop continually
 * @param anchor The actual position of this animation in relation to the actor using it
 *
 * Doxygen doesn't show the fourth parameter to this function for some reason. it is (Vector anchor) and defaults to {0,0,0}
 */
int Animation::setStrip(SpriteStrip *strip, int fps, int loop, Vector anchor)
{
    if ( strip == NULL ) {
	std::cerr << "Got NULL sprite strip\n";
	return 1;
    }
    this->loop = loop;
    this->strip = strip;
    //std::cerr << "this->strip is now " << strip << "\n";
    this->fps = fps;
    if ( this-> fps > 0 )
	this->timeStep = 1000 / fps;
    if ( anchor.x <= this->strip->width() )
        this->anchor.x = anchor.x;
    if ( anchor.y <= this->strip->height() )
	this->anchor.y = anchor.y;
    return 0;
}

/**
 * @brief Return the next frame of this animation to be rendered
 * @param curFrame Frame to render, or 0 if actorSource is set
 * @param lastTime Last update time, or 0 if actorSource is set
 * @param actorSource The actor for which this animation is being rendered, or NULL
 * 
 * Performs logic necessary to check time from lastTime vs current time to return the
 * appropriate frame. If actorSource is set, it will pull a FrameCounter from that actor
 * to use for curFrame/lastTime.
 *
 * FIXME : The way this interaction works is INCREDIBLY hackish. I should look
 * at collapsing the nextFrame() functionality into Actor, and basically redoing the
 * Animation class as a structure that does nothing. For now, though, this works.
 *
 * @return (SDL_Surface *) Will return the first frame of animation if the animation is stopped, otherwise will return the current frame. Will only return NULL if the Animation has no frames assigned.
 */

SDL_Surface *Animation::nextFrame(int curFrame, int lastTime, AnimatedRenderable *actorSource)
{
    FrameCounter fc;
    if ( actorSource ) {
	fc = actorSource->getFrameCounter();
	curFrame = fc.curFrame;
	lastTime = fc.lastTime;
    }
    SDL_Surface *toRet = NULL;
    //std::cerr << "Checking for step; timeStep " << this->timeStep << " lastTime " << lastTime << "\n";
    if ( this->fps > 0 && ((SDL_GetTicks() - lastTime) >= this->timeStep) ) {
	if ( curFrame +1 >= this->strip->numFrames() && this->loop == 0 ) {
	    toRet = this->strip->getFrame(0);
	    //std::cerr << "Time for a step; Returning frame 0\n";
	} else if ( curFrame + 1 >= this->strip->numFrames() && this->loop == 1) {
	    return NULL;
	}
	//std::cerr << "Time for a step; returning frame " << curFrame + 1 << "\n";
	toRet = this->strip->getFrame(curFrame+1);
	curFrame += 1;
	if ( actorSource ) {
	    fc.curFrame += 1;
	    if ( fc.curFrame >= this->strip->numFrames() && this->loop == 0 ) {
		//std::cerr << "Resetting to zero\n";
	        fc.curFrame = 0;
		curFrame = 0;
	    }
	    fc.lastTime = SDL_GetTicks();
	    fc.lastFrame = toRet;
	    actorSource->setFrameCounter(fc);
	}
    } else if ( this->fps == 0 ) {
	//std::cerr << "0 FPS ; Returning frame 0\n";
	toRet = this->strip->getFrame(0);
    } else {
	//std::cerr << "Not time for a step; Returning frame " << curFrame << "\n";
	toRet = this->strip->getFrame(curFrame);
    }
    if ( toRet == NULL )
	return this->strip->getFrame(0);
    else 
	return toRet;
}

/**
 * @brief return the number of frames in this animation
 * @return integer
 */

int Animation::numFrames()
{
    if ( this->strip != NULL ) { 
	return this->strip->numFrames();
    }
    return 0;
}

/**
 * @brief Return a new vector modified by the anchor vector
 * @return a new vector
 */
Vector Animation::anchorAt(Vector position)
{
    Vector newVector;
    newVector.x = position.x - this->anchor.x;
    newVector.y = position.y - this->anchor.y;
    return newVector;
}
