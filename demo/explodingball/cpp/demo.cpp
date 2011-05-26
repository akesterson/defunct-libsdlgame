/*
 * This demo expands on the frictionball demo. It shows how to create objects dynamically, by 
 * creating an explosion every time the ball touches the ground
 */

#include <libgame/libgame.h>
#include <string>

int main(int argc, char *argv[])
{
	Display2D display = Display2D();
	Game &myGame = Game::NewSingleton();
	Actor ball; 
	Actor *exploder = NULL;
	float gravity = 0.1;
	//int exploderCount = 0;
	unsigned int lastTimer = 0;

	Vector actorPos;
	Vector actorVel;

	myGame.initSDL();
	myGame.initVideo(640, 480, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	display.initVideo((Vector){0,0,0}, 640, 480, 32, SDL_HWSURFACE);
	display.setActive(1);
	myGame.windows.push_back(&display);

	// this stuff is a bit more terse now just to save space

        myGame.newSpriteStrip("ball")->loadFromFile("ball.png", 32, 32, (Vector){0,0,0});
	myGame.newAnimation("ball")->setStrip(myGame.getSpriteStrip("ball"), 0, 0, (Vector){0,0,0});
	// create the new animation for the explosion
	myGame.newSpriteStrip("explosion")->loadFromFile("explosion.png", 110, 110, (Vector){0,0,0});
	myGame.newAnimation("explosion")->setStrip(myGame.getSpriteStrip("explosion"), 12, 0, (Vector){0,0,0});
	ball.addAnimation(myGame.getAnimation("ball"), STATE_DEFAULT);
	ball.setState(STATE_MOVERIGHT | STATE_MOVEUP);
	ball.setPosition((Vector){0,400,0});
	ball.setVelocity((Vector){.25, 1.5, 0});
	display.addActor(&ball, LAYER_SPRITE1);

	lastTimer = SDL_GetTicks();

	while ( 1 ) {
	    actorPos = ball.getPosition();
	    actorVel = ball.getVelocity();
	    /* this is what does the bounce */
	    if ( actorPos.y > (480 - ball.nextFrame()->h) && (actorVel.y != 0 ) ) {
		// if the velocity is low enough, then we just stop the ball so that it will roll
		// smoothly along the floor of the window
		if ( actorVel.y < .25 && actorVel.y > -.25 ) {
		    //std::cerr << "STOPPING BALL\n";
		    actorVel.y = 0;
		    ball.setState(STATE_NONE);
		} else {
		    // if the velocity is still significant, then we reverse it to create the bounce
		    // but since real balls don't bounce quite as high as they did initially, when dropped
		    // so we do a primitive modification of the velocity to inroduce something similar to
		    // friction physics (see the "physics" demos for ACTUAL physics)

		    // The friction numbers here are arbitrary, and were used because they made sense in the
		    // demo, you can change the friction for some pretty different effects.
		    actorVel.y = -(actorVel.y) - .25;
		    // this is friction applied to the X axis to stop the ball's movement eventually
		    if ( actorVel.x < .01 && actorVel.x > -.01 ) {
			actorVel.x = 0;
		    } else {
			actorVel.x -= .025;
		    }
		}
		exploder = myGame.newActor();
		exploder->addAnimation(myGame.getAnimation("explosion"), STATE_DYING);
		exploder->setState(STATE_DYING);
		exploder->setPosition(actorPos);
		display.addActor(exploder, LAYER_SPRITE2);
                
		actorPos.y = (480 - ball.nextFrame()->h);
		ball.setPosition(actorPos);
	    }
	    // -- end of bouncing
	    if ( actorPos.x >= 640 ) {
		break;
	    }
	    if ( (SDL_GetTicks() - lastTimer) >= (1000/30) ) {
		myGame.update();
		if ( ball.hasState(STATE_MOVEUP) ) {
		    actorVel.y -= gravity;
		}
		//std::cerr << "Velocity Y " << actorVel.y << "\n";
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
