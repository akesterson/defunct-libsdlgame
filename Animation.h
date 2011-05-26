#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <SDL.h>
#include "SpriteStrip.h"
#include "Renderable.h"
#include <map>

class Animation
{
private:
    unsigned int lastTime;
    SpriteStrip *strip;
    unsigned int timeStep;
    int fps;
    int loop;
    Vector anchor; 
public:
    Animation();
    virtual ~Animation();
    int getLoop() { return this->loop; };
    int setStrip(SpriteStrip *strip, int fps, int loop = 0, Vector anchor = (Vector){0,0,0});
    SDL_Surface *nextFrame(int curFrame, int lastTime, AnimatedRenderable *actorSource = NULL);
    int numFrames();
    Vector anchorAt(Vector position);
};

//typedef std::map<std::string, Animation *> AnimationMap;
//typedef std::map<std::string, Animation *>::iterator AnimationMapIterator;
typedef std::map<unsigned int, Animation *> AnimationStateMap;
typedef std::map<unsigned int, Animation *>::iterator AnimationStateMapIterator;


#endif // __ANIMATION_H__
