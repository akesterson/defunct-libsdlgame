/*
 * This demo starts up and does nothing until you hit Escape to kill it.
 */

#include <libsdlgame/libsdlgame.h>

int main(int argc, char *argv[])
{
	Display2D display = Display2D();
	Game &myGame = Game::NewSingleton();
	unsigned int lastTimer = 0;

	myGame.initSDL();
	myGame.initVideo(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	display.initVideo((Vector){0,0,0}, 640, 480, 32, SDL_HWSURFACE);
	display.setActive(1);
	myGame.windows.push_back(&display);

	lastTimer = SDL_GetTicks();

	while ( 1 ) {
	    if ( (SDL_GetTicks() - lastTimer) >= (1000/30) ) {
		myGame.update();
		lastTimer = SDL_GetTicks();
	    } else {
		myGame.update(1);
	    }
	    if ( myGame.keyHeldDown(SDLK_ESCAPE) ) {
		break;
	    }
	    myGame.finishFrame();
	}
}
