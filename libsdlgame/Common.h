#ifndef __COMMON_H__
#define __COMMON_H__

#include <SDL.h>

/**
 * @def GAMEFPS
 * Redefine this value to change the FPS you want the game to lock to 
 * (USUALLY works)
*/
#define GAMEFPS		60

/**
 * @struct Vector
 * @brief A simple way of representing a 3-dimensional point or vector
 */
struct Vector
{
    float x;
    float y;
    float z;
};

/**
 * @typedef Point
 * @brief A convenient typedef for Vector
 */
typedef Vector Point;

/**
 * @struct FrameCounter
 * @brief A structure to use for counting frame times and positions
 */
struct FrameCounter
{
  unsigned int curFrame; /**< current frame */
  unsigned int lastTime; /**< ticks at last logic */
  SDL_Surface *lastFrame; /**< previous frame returned by this counter */
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
  SDL_Surface *canvas; /**< actual SDL_Surface used by this SharedCanvas */
public:
    SharedCanvas2D();
    virtual ~SharedCanvas2D();
    SDL_Surface *getCanvas();
    virtual int shareCanvas(SharedCanvas2D *obj);
    virtual int shareCanvas(SDL_Surface *obj);
    virtual int releaseCanvas();
};

#endif // __COMMON_H__
