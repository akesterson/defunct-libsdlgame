#ifndef __FONTRENDERER_H__
#define __FONTRENDERER_H__

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <map>
#include "Common.h"

class FontRenderer
{
protected:
    // organized by loadedFonts[FontName][PointSize] = TTF_Font *
    FontRenderer();
    ~FontRenderer();
    std::map<std::string, std::map<int, TTF_Font *> > loadedFonts;
    SDL_Color textColor;
    SDL_Color bgColor;
    int alpha;
public:
    static FontRenderer &NewSingleton()
    {
		static FontRenderer rend;
		return rend;
    }
    void setColor(SDL_Color textColor, SDL_Color bgColor, int alpha);
    int loadFont(std::string filename, std::string fontname, int pointsize);
    SDL_Rect renderString(std::string text, 
		      SDL_Surface *dest, 
		      std::string fontname,  
		      Vector position, 
		      int pointsize);
};

#endif // __FONTRENDERER_H__
