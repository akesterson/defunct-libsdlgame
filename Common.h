#ifndef __COMMON_H__
#define __COMMON_H__

#include <SDL.h>

#define GAMEFPS		60

struct Vector
{
    float x;
    float y;
    float z;
};

typedef Vector Point;

struct FrameCounter
{
    unsigned int curFrame;
    unsigned int lastTime;
    SDL_Surface *lastFrame;
};

/**
 * @class SharedCanvas2D
 * @brief A class that allows multiple objects to re-use a single SDL_Surface with reference counting for safe memory usage
 *
 * The only thing that might be confusing about this class is that it is just
 * a container for an SDL_Surface that implements refcounting.
 */

class SharedCanvas2D
{
protected:
    SDL_Surface *canvas;
public:
    SharedCanvas2D();
    virtual ~SharedCanvas2D();
    SDL_Surface *getCanvas();
    virtual int shareCanvas(SharedCanvas2D *obj);
    virtual int shareCanvas(SDL_Surface *obj);
    virtual int releaseCanvas();
};

#endif // __COMMON_H__
