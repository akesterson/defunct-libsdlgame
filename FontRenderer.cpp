#include "FontRenderer.h"
#include "Common.h"
#include <SDL.h>
#include <SDL_gfxPrimitives.h>

FontRenderer::FontRenderer()
{
    this->setColor((SDL_Color) {0,0,0,0}, (SDL_Color){255,255,255,0}, 1);
}

FontRenderer::~FontRenderer()
{
    ////std::cerr << "Freeing memory in FontRenderer is not yet implemented\n";
}

int FontRenderer::loadFont(std::string filename, std::string fontname, int pointsize)
{
    TTF_Font *font = TTF_OpenFont(filename.c_str(), pointsize);
    if ( font == NULL ) {
	////std::cerr << "Unable to load font " << filename << "\n";
	return 1;
    }
    this->loadedFonts[fontname][pointsize] = font;
    return 0;
}

void FontRenderer::setColor(SDL_Color textColor, SDL_Color bgColor, int alpha)
{
    //std::cerr << "FontRenderer::setColor (" << textColor.r << " : " << textColor.g << " : " << textColor.b << ") "
	//<< " ( " << bgColor.r << " : " << bgColor.g << " : " << bgColor.b << " ) alpha " << alpha << "\n";
    this->textColor = textColor;
    this->bgColor = bgColor;
    this->alpha = alpha;
}

SDL_Rect FontRenderer::renderString(std::string text, 
			  SDL_Surface *dest, 
			  std::string fontname,
			  Vector position,
			  int pointsize)
{
    SDL_Rect destrect;
    SDL_Surface *surface;
    SDL_Color foregroundColor, backgroundColor;
    TTF_Font *font;
    SDL_Rect blankrect;
    int plainTextWidth = 0;
    
    if ( dest == NULL ) {
	return blankrect;
    }

    if ( fontname == "" ) {
	plainTextWidth = text.size() * 8;
	if ( position.x == -1 )
	    position.x = (dest->w/2) - (plainTextWidth/2);
        if ( position.y == -1 )
	    position.y = (dest->h/2) - 8;
	//std::cerr << "Rendering (plain text) " << text << " at " << position.x << " x " << position.y << "\n";
	stringRGBA(dest, (int)position.x, (int)position.y, text.c_str(), 
		   this->textColor.r, this->textColor.g, this->textColor.b, this->alpha); 
	return blankrect;
    }
    ////std::cerr << "Rendering (true type) " << text << " at " << position.x << " x " << position.y << "\n";
    if ( this->loadedFonts.count(fontname) == 1 ) {
	if ( this->loadedFonts[fontname].count(pointsize) == 1 ) {
	    ////std::cerr << "Found font..";
	    font = this->loadedFonts[fontname][pointsize];
	} else {
	    ////std::cerr << "No font found for (point size) 20\n";
	    return blankrect;
	}
    } else {
	////std::cerr << "No font found for (" << fontname << ")\n";
	return blankrect;
    }
    surface = TTF_RenderUTF8_Shaded(font, text.c_str(), this->textColor, this->bgColor);
    
    if (surface == NULL)
    {
        ////std::cerr << "Couldn't create String " << text << ":" << SDL_GetError() << "\n";
	return blankrect;
    }
    
    /* Blit the entire surface to the screen */

    destrect.x = (position.x == -1 ? (dest->w - surface->w) / 2 : (Sint16) position.x);
    destrect.y = (position.y == -1 ? (dest->h - surface->h) / 2 : (Sint16) position.y);
    destrect.w = surface->w;
    destrect.h = surface->h;

    ////std::cerr << "FontRenderer::renderString this->alpha " << this->alpha << "\n";
    if ( this->alpha == 1 ) {
        SDL_SetColorKey(surface, SDL_SRCCOLORKEY, 
			SDL_MapRGBA(surface->format, this->bgColor.r, this->bgColor.g, this->bgColor.b, 0) );
    }
    SDL_BlitSurface(surface, NULL, dest, &destrect);
    
    /* Free the generated string image */

    SDL_FreeSurface(surface);
    return destrect;
}
