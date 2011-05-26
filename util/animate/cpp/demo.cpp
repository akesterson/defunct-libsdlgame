#include <libgame/libgame.h>
#include <string>
#include <SDL_gfxPrimitives.h>
#include <cstdlib>
#include <sstream>

int main(int argc, char *argv[])
{
    FontRenderer &font = FontRenderer::NewSingleton();
    char gimmeitoa[32];
    Display2D display = Display2D();
	Game &myGame = Game::NewSingleton();
	Animation *anim = NULL;
	Actor animator;
	Vector velocity;
	Vector position;
	SpriteStrip *strip = NULL;

	if ( argc < 9 ) {
	    std::cerr << "animate SCRX SCRY SCRDEPTH IMAGEFILE FRAMEWIDTH FRAMEHEIGHT FPS LOOP [XVEL] [YVEL]\n";
	    exit(1);
	}

	myGame.initSDL();
	myGame.initVideo(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), SDL_HWSURFACE|SDL_DOUBLEBUF);
	//display.initVideo((Vector){0,0,0}, atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), SDL_HWSURFACE);
	display.shareCanvas(&myGame);
	display.setActive(1);
	myGame.windows.push_back(&display);

	strip = myGame.newSpriteStrip("anim");
	if ( strip->loadFromFile(argv[4], atoi(argv[5]), atoi(argv[6]), (Vector){0,0,0}) ) {
	    std::cerr << "Failed to load sprite from file " << argv[4] << " (framesize " << argv[5] << "x" << argv[6] << "\n";
	    exit(1);
	}
	anim = myGame.newAnimation("anim");
	anim->setStrip(strip, atoi(argv[7]), atoi(argv[8]), (Vector){0,0,0});
	animator.addAnimation(anim, STATE_DEFAULT);
	animator.setState(STATE_NONE);
	if ( argc >= 10) {
	    animator.addState(STATE_MOVEXAXIS);
	    velocity.x = atof(argv[9]);
	}
	if ( argc >= 11 ) {
	    animator.addState(STATE_MOVEYAXIS);
	    velocity.y = atof(argv[10]);
	}
	animator.setVelocity(velocity);

	animator.setPosition((Vector){ 
	    (myGame.getCanvas()->w/2)-((animator.nextFrame()->w)/2),
	    (myGame.getCanvas()->h/2)-((animator.nextFrame()->h)/2),
	    0} );

	display.addActor(&animator, LAYER_SPRITE1);
        font.setColor((SDL_Color){255, 255, 255, 255}, (SDL_Color){0, 0, 0, 255}, 255);
	myGame.lockFPS(30);
	std::string msg;

	while ( 1 ) {
	    msg = argv[4];
            msg += " ";
	    sprintf((char *)&gimmeitoa, "(%dx%d) %s/%d FPS", 
		    animator.nextFrame()->w, 
		    animator.nextFrame()->h,
		    argv[7],
		    myGame.realfps);
	    msg += (char *)&gimmeitoa;
	    //std::cerr << msg << "\n";
	    myGame.update();
	    font.renderString(msg, myGame.getCanvas(), "", (Vector){-1, 20, 0}, 0);
	    if ( myGame.keyHeldDown(SDLK_ESCAPE) ) {
		break;
	    }
	    position = animator.getPosition();
	    if ( position.x < 0 || position.x > 640 ) {
		position.x = (myGame.getCanvas()->w/2)-((animator.nextFrame()->w)/2);
	    }
	    if ( position.y < 0 || position.y > 480 ) {
		position.y = (myGame.getCanvas()->h/2)-((animator.nextFrame()->h)/2);
	    }
	    animator.setPosition(position);
	    myGame.finishFrame();
	}
}
