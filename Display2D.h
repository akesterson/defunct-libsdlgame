#ifndef __DISPLAY2D_H__
#define __DISPLAY2D_H__

#include "Display.h"
#include "Common.h"

#define LAYER_BACKGROUND	0
#define LAYER_SPRITE1		1
#define LAYER_SPRITE2		2
#define LAYER_SPRITE3		3
#define LAYER_EFFECTS		4
#define LAYER_HUDOVERLAY	5
#define LAYER_MENUOVERLAY	6

#define MAX_LAYERS		6

class Display2D : public Display
{
protected:
    int blitSprite(SDL_Surface *srcframe, int x, int y);
    std::vector<Actor *> layers[MAX_LAYERS];
public:
    Display2D();
    int addActor(Actor *actor, int layer = LAYER_BACKGROUND);
    void update(int logicOnly = 0);
    void setTransparentBG();
};

#endif // __Display2D_H__
