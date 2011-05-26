#ifndef __SPRITESTRIP_H__
#define __SPRITESTRIP_H__

#include <SDL.h>
#include <vector>
#include <string>
#include <map>
#include "Renderable.h"

typedef std::vector<SDL_Surface *>::iterator SpriteStripIter;

class SpriteStrip 
{
private:
    std::vector<SDL_Surface *> frames;
public:
    SpriteStrip();
    ~SpriteStrip();
    int width();
    int height();
    int loadFromFile(std::string filename, int f_w = 0, int f_h = 0, Vector colorkey = (Vector){-1,-1,-1});
    int loadFromSurface(SDL_Surface *image, int f_w = 0, int f_h = 0, Vector colorkey = (Vector){-1,-1,-1});
    void clearFrames();
    SDL_Surface *getFrame(unsigned int frame);
    int numFrames();
    SpriteStripIter iterator();
};

typedef std::map<std::string, SpriteStrip *> SpriteStripMap;


#endif // #ifndef __SPRITESTRIP_H__
