/*
 * This demo just creates a ball that moves up, and allows gravity to effect it.
 * The effect is to cause the velocity of the ball to change, causing it to travel
 * in a parabola on the Y axis rather than simply going straight up or straight down.
 */

#include <libgame/libgame.h>

int main(int argc, char *argv[])
{
	Display2D display = Display2D();
	Game &myGame = Game::NewSingleton();
	Animation *anim = NULL;
	Actor ball;
	SpriteStrip *strip = NULL;
	float gravity = 0.1;
	unsigned int lastTimer = 0;

	Vector actorPos;
	Vector actorVel;

	myGame.initSDL();
	myGame.initVideo(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	display.initVideo((Vector){0,0,0}, 640, 480, 32, SDL_HWSURFACE);
	display.setActive(1);
	myGame.windows.push_back(&display);

	strip = myGame.newSpriteStrip("ball");
	strip->loadFromFile("ball.png", 32, 32, (Vector){0,0,0});
	anim = myGame.newAnimation("ball");
	anim->setStrip(strip, 0, 0, (Vector){0,0,0});
	ball.addAnimation(anim, STATE_DEFAULT);
	ball.setState(STATE_MOVERIGHT | STATE_MOVEUP);
	ball.setPosition((Vector){0,400,0});
	ball.setVelocity((Vector){1, 1.5, 0});

	display.addActor(&ball, LAYER_SPRITE1);

	lastTimer = SDL_GetTicks();

	while ( 1 ) {
	    actorPos = ball.getPosition();
	    if ( actorPos.y >= 480 ) {
		break;
	    }
	    if ( (SDL_GetTicks() - lastTimer) >= (1000/30) ) {
		myGame.update();
		actorVel = ball.getVelocity();
		actorVel.y -= gravity;
		ball.setVelocity(actorVel);
		lastTimer = SDL_GetTicks();
	    } else {
		myGame.update(1);
	    }
	    if ( myGame.keyHeldDown(SDLK_ESCAPE) ) {
		break;
	    }
	    myGame.finishFrame();
	}
	return 0;
}
