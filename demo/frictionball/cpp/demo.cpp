/*
 * This demo expands on the bouncingball demo to show how you can integrate simple physics to create a
 * more realistic bouncing effect, and bring the ball to a halt
 */

#include <libsdlgame/libsdlgame.h>

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
	ball.setPosition((Vector){320,240,0});
	ball.setVelocity((Vector){-1, 0, 0});

	display.addActor(&ball, LAYER_SPRITE1);

	lastTimer = SDL_GetTicks();
	myGame.lockFPS(60);

	while ( 1 ) {
	    actorPos = ball.getPosition();
	    actorVel = ball.getVelocity();
	    /* this is what does the bounce */
	    if ( actorPos.y > (480 - ball.nextFrame()->h) ) {
		// if the velocity is low enough, then we just stop the ball so that it will roll
		// smoothly along the floor of the window
		if ( actorVel.y < .01 && actorVel.y > -.01 ) {
		    actorVel.y = 0;
		} else {
		    // if the velocity is still significant, then we reverse it to create the bounce
		    // but since real balls don't bounce quite as high as they did initially, when dropped
		    // so we do a primitive modification of the velocity to inroduce something similar to
		    // friction physics (see the "physics" demos for ACTUAL physics)

		    // The friction numbers here are arbitrary, and were used because they made sense in the
		    // demo, you can change the friction for some pretty different effects.
		    actorVel.y = -(actorVel.y) - 1;
		    // this is friction applied to the X axis to stop the ball's movement eventually
		    if ( actorVel.x < .01 && actorVel.x > -.01 ) {
			actorVel.x = 0;
		    } else {
			if ( actorVel.x > 0  ) {
			    actorVel.x -= .1;
			} else {
			    actorVel.x += .1;
			}
		    }
		}
		actorPos.y = (480 - ball.nextFrame()->h);
		ball.setPosition(actorPos);
	    }
	    if ( ((actorPos.x + ball.nextFrame()->w) >= 640) || actorPos.x < 0 ) {
		actorVel.x = -(actorVel.x);
	    }
	    // -- end of bouncing
	    if ( actorPos.x >= 640 ) {
		break;
	    }
	    if ( (SDL_GetTicks() - lastTimer) >= (1000/30) ) {
		actorVel.y -= gravity;
		ball.setVelocity(actorVel);
		lastTimer = SDL_GetTicks();
	    }
	    myGame.update();
	    if ( myGame.keyHeldDown(SDLK_ESCAPE) ) {
		break;
	    }
	    myGame.finishFrame();
	}
	return 0;
}
