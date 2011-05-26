#include "Renderable.h"
#include <SDL.h>
#include <iostream>

Renderable::Renderable()
{
    this->position = (Vector){0,0,0};
    this->velocity = (Vector){0,0,0};
}

Vector Renderable::getPosition()
{
    return this->position;
}

Vector Renderable::getVelocity()
{
    return this->velocity;
}

void Renderable::setPosition(Vector pos)
{
    this->position.x = pos.x;
    this->position.y = pos.y;
    this->position.z = pos.z;
}

void Renderable::setVelocity(Vector vel)
{
    this->velocity.x = vel.x;
    this->velocity.y = vel.y;
    this->velocity.z = vel.z;
}

/**
 * @fn Renderable::nextFrame
 * @brief Return the next surface that this class is ready to render
 * @return SDL_Surface *
 */

SDL_Surface *Renderable::nextFrame()
{
    return NULL;
}

AnimatedRenderable::AnimatedRenderable()
{
    this->frameCounter.curFrame = 0;
    this->frameCounter.lastFrame = NULL;
    this->frameCounter.lastTime = 0;
}


FrameCounter AnimatedRenderable::getFrameCounter()
{
    return this->frameCounter;
}

/**
 * @fn AnimatedRenderable::setFrameCounter
 * @brief Force the frame counter to a given FrameCounter value
 * @return 0
 */
int AnimatedRenderable::setFrameCounter(FrameCounter fc)
{
    ////std::cerr << "Updating frame counter\n";
    this->frameCounter = fc;
    return 0;
}
