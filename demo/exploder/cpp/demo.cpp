#include <libsdlgame/libsdlgame.h>

int main(int argc, char *argv[])
{
	Display2D display = Display2D();
	Game &myGame = Game::NewSingleton();
	Animation *anim = NULL;
	Actor exploder;
	SpriteStrip *strip = NULL;

	myGame.initSDL();
	myGame.initVideo(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	display.initVideo((Vector){0,0,0}, 640, 480, 32, SDL_HWSURFACE);
	display.setActive(1);
	myGame.windows.push_back(&display);

	strip = myGame.newSpriteStrip("explosion");
	strip->loadFromFile("explosion.png", 110, 110, (Vector){0,0,0});
	anim = myGame.newAnimation("explosion");
	anim->setStrip(strip, 12, 0, (Vector){0,0,0});
	exploder.addAnimation(anim, STATE_DEFAULT);
	exploder.setState(STATE_NONE);

	display.addActor(&exploder, LAYER_SPRITE1);

	while ( 1 ) {
		myGame.update();
		if ( myGame.keyHeldDown(SDLK_ESCAPE) ) {
			break;
		}
		myGame.finishFrame();
	}
	return 0;
}
