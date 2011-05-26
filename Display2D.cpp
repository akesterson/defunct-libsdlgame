#include "Display2D.h"
#include "Renderable.h"
#include <SDL.h>
#include <iostream>

Display2D::Display2D()
{
    return;
}

int Display2D::addActor(Actor *actor, int layer)
{
    Display::addActor(actor);
    this->layers[layer].push_back(actor);
    //std::cerr << "Added actor " << actor << " to layer " << layer << "\n";
    return 0;
}

void Display2D::update(int logicOnly)
{
    ActorListIterator actorIter;
    ActorListIterator newActorIter;
    SDL_Surface *curFrame = NULL;
    Actor *actor = NULL;

    if ( this->active == 0 ) {
	return;
    }

    //std::cerr << "Updating Display2D " << this << "\n";

    SDL_Rect screen = {0, 0, this->canvas->w, this->canvas->h};
    Vector actorpos;
    if ( this->canvas->refcount < 2 ) {
	// don't blank out someone's shared canvas
	SDL_FillRect(this->canvas, &screen, 0);
    }
    for ( int layer = 0; layer < MAX_LAYERS; layer++ ) {
	//std::cerr << "Displaying " << this->layers[layer].size() << " sprites on layer " << layer << " on canvas " << this->canvas << "\n";
	actorIter = this->layers[layer].begin();
	while (  actorIter != this->layers[layer].end() ) {
	    actor = (*actorIter);
	    //std::cerr << "Updating actor " << actor << "\n";
	    if ( actor != NULL ) { 
		actor->update();
		if ( logicOnly == 0 ) {
		    curFrame = actor->nextFrame();
		    actorpos = actor->getPosition(1);
		    //std::cerr << "Position of " << actor << " " << actorpos.x << "x" << actorpos.y << "\n";
		    if ( curFrame ) {
			//std::cerr << "Blitting sprite " << curFrame << " to (" << actorpos.x << "," << actorpos.y << ")...\n";
			SDL_Rect srcRect = {0, 0, curFrame->w, curFrame->h};
			SDL_Rect destRect = {(Sint16) actorpos.x, (Sint16)actorpos.y, 0, 0};
			SDL_BlitSurface(curFrame, &srcRect, this->canvas, &destRect);
		    } 
		} 
		// more logic
		if ( actor->hasState(STATE_DEAD) ) {     
		    actorIter = this->layers[layer].erase(actorIter);
		    newActorIter = this->actors.begin();
                    while (  newActorIter < this->actors.begin() ) {
			if ( *newActorIter == actor ) {
			    newActorIter = this->actors.erase(newActorIter);
			    continue;
			}
			newActorIter++;
		    }
		    //delete actor;
		    continue;
		}
	    }
	    actorIter++;
	}
    }
}

void Display2D::setTransparentBG()
{ 
	SDL_SetColorKey(this->canvas, SDL_SRCCOLORKEY, SDL_MapRGB(this->canvas->format, 0, 0, 0));   
}

