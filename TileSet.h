#ifndef __TILESET_H__
#define __TILESET_H__

#include <SDL.h>
#include "Display2D.h"

#define TILEMAP_ORTHOGONAL	1
#define TILEMAP_ISOMETRIC	2

typedef std::map<int, SDL_Surface *> TileSet;

struct TileSetDef {
    int start_id;
    std::string name;
    TileSet *tiles;
}

typedef std::vector<TileSetDef *> TileGroup;
typedef std::vector<int> TileLayer;

class TileDisplay : public Display2D {
protected:
    TileGroup tilegroup;
    std::vector<int>[MAX_LAYERS] layers;
    int w;
    int h;
    int tw;
    int th;
public:
    int loadFromTMX(std::string filename);
    void clearTiles();
    void scroll(int x, int y);
    void autoScroll(int x, int y);
    void update();
}

#endif // __TILESET_H__
