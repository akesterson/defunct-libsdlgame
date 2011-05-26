#include "Game.h"
#include "SpriteStrip.h"
#include "Actor.h"
#include "Display2D.h"
#include <SDL_mixer.h>
#include <SDL.h>
#include <iostream>

Game::Game()
{
	this->musicVolume = 128;
	this->soundVolume = 128;
	this->lastNumKeys = 0;
	this->lastKeyState = NULL;
	this->fpslock = 0;
	this->realfps = 0;
	return;
}

Game::~Game()
{
	if ( this->canvas != NULL ) {
		SDL_FreeSurface(this->canvas);
	}

}

int Game::initSDL(int audio_rate, Uint16 audio_format, int audio_channels, int audio_buffers)
{
	if ( SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0 ) {
		//std::cerr << "Failed to initialize SDL. Error was: " << SDL_GetError() << "\n";
		return 1;
	}

	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0) {
		//std::cerr << "Unable to initialize audio: " << Mix_GetError() << "\n";
		exit(1);
	}
	return 0;
}

int Game::initVideo(int w, int h, int depth, int flags)
{
	this->canvas = SDL_SetVideoMode(w, h, depth, flags);
	if ( this->canvas == NULL ) {
		//std::cerr << "Failed to create new SDL display. Error was: " << SDL_GetError() << "\n";
		return 1;
	}
	////std::cerr << "Created new SDL display at " << this->canvas << "\n";
	return 0;
}

void Game::update(int logicOnly)
{
	DisplayList::iterator displayIter;
	SDL_Surface *curFrame = NULL;
	Display *display = NULL;
	Uint8 *keyState = NULL;
	int numKeys = 0;
	int gfxframe = 1;
	static unsigned int lastTime = 0;
	static unsigned int lastfps = 0;
	static unsigned int realFPStimer = 0;
	SDL_Rect screen = {0, 0, this->canvas->w, this->canvas->h};
	Vector displaypos;
	
	SDL_PumpEvents();

	if ( (this->fpslock != 0) && (SDL_GetTicks() - lastTime) >= (1000/this->fpslock) ) {
		gfxframe = 1;
		lastTime = SDL_GetTicks();
	} else if (this->fpslock != 0) {
	  return;
	}

	if ( (SDL_GetTicks() - realFPStimer) > 1000 ) {
		this->realfps = lastfps;
		lastfps = 0;
		realFPStimer = SDL_GetTicks();
	} else {
		lastfps++;
	}

	if ( logicOnly == 0 ) {
		SDL_FillRect(this->canvas, &screen, 0);
	}
	for ( displayIter = this->windows.begin(); displayIter != this->windows.end() ; displayIter++ ) {
		display = (*displayIter);
		//std::cerr << "Updating display " << display << "\n";
		display->update();
		if ( logicOnly == 1 ) {
			continue;
		}

		curFrame = display->nextFrame();
		displaypos = display->getPosition();
		//std::cerr << "Position of " << display << " " << displaypos.x << "x" << displaypos.y << "\n";
		if ( curFrame && curFrame != this->canvas ) {
		  //std::cerr << "Blitting display " << display << " canvas " << curFrame << " to main canvas " << this->canvas << "(" << displaypos.x << "," << displaypos.y << ")...\n";
		  SDL_Rect destRect = {(Sint16) displaypos.x, (Sint16) displaypos.y, 0, 0};
		  SDL_BlitSurface(curFrame, NULL, this->canvas, &destRect);
		  //this->blitSprite(curFrame, 0, 0);
		} else if ( curFrame == this->canvas ) {
		  //std::cerr << "Not blitting display window " << display << " to our canvas " << this->canvas << " because it is sharing our canvas at " << curFrame << "\n";
		} else if ( curFrame == NULL ) {
		  //std::cerr << "display " << display << " has a NULL canvas\n";
		}
	}

	if ( this->lastKeyState != NULL ) {
		delete this->lastKeyState;
	}
	keyState = SDL_GetKeyState(&numKeys);
	this->lastKeyState = new Uint8[numKeys];
	if ( this->lastKeyState != NULL ) {
		memcpy(this->lastKeyState, keyState, (sizeof(Uint8)*numKeys));
	}

	//this->handleEvents();
	this->cleanSounds();
}

/*int Game::handleControlEvent(SDL_Event *event)
{
	switch(event->type) {
	case SDL_KEYUP:
	if ( event->key.state == SDL_RELEASED ) {
		if ( event->key.keysym.sym == SDLK_ESCAPE ) {
		exit(0);
		}
	}
	}
	return 0;
}

int Game::handleEvents()
{
	SDL_Event nextEvent;
	while ( SDL_PollEvent(&nextEvent) ) {
	switch(nextEvent.type) {
	case SDL_KEYUP:
		this->handleControlEvent(&nextEvent);
	}
	}
	return 0;
}
*/
int Game::playMusic(std::string filename)
{
	Mix_HaltMusic();
	if ( this->bgmusic )
		Mix_FreeMusic(this->bgmusic);
	this->bgmusic = Mix_LoadMUS(filename.c_str());
	if ( this->bgmusic == NULL ) {
		//std::cerr << "Unable to load " << filename.c_str() << " to play music.\n";
		return 1;
	}
	if ( Mix_PlayMusic(this->bgmusic, -1) == -1 ) {
		//std::cerr << "Unable to play background music. " << Mix_GetError() << " \n";
		return 1;
	}
	Mix_VolumeMusic(this->musicVolume);
	return 0;
}

void Game::haltMusic()
{
	Mix_HaltMusic();
	if ( this->bgmusic )
		Mix_FreeMusic(this->bgmusic);
	this->bgmusic = NULL;
}

int Game::playSound(std::string filename, int loops)
{
	PlayingSound *sndstruct = NULL;     
	Mix_Chunk *sound = NULL;
	int channel = 0;

	sound = Mix_LoadWAV(filename.c_str());
	if ( sound == NULL ) {
		return 1;
	}

	channel = Mix_PlayChannel(-1, sound, loops);
	if ( channel > -1 ) {
		//std::cerr << "Pushing sound chunk " << sound << " on channel " << channel << "\n";
		sndstruct = new PlayingSound();
		sndstruct->sound = sound;
		sndstruct->channel = channel;
		this->playingSounds.push_back(sndstruct);
	} else {
		Mix_FreeChunk(sound);
		return 1;
	}
	Mix_Volume(sndstruct->channel, this->soundVolume);
	return 0;
}

void Game::cleanSounds(int force)
{
	PlayingSoundList::iterator iter;
	PlayingSoundList::iterator newIter;
	PlayingSound *curSound = NULL;
	iter = this->playingSounds.begin() ; 

	while ( iter != this->playingSounds.end()) {
		curSound = *iter;
		if ( curSound == NULL ) {
			iter++;
			continue;
		}

		if ( Mix_Playing(curSound->channel) != 0 && force == 1) {
			Mix_HaltChannel(curSound->channel);
		} else if ( Mix_Playing(curSound->channel) != 0 ) {
			iter++;
			continue;
		}

		newIter = iter;
		iter++;
		Mix_FreeChunk(curSound->sound);
		//std::cerr << "Cleared sound at channel" << curSound->channel << "\n";
		iter = this->playingSounds.erase(newIter);
		delete curSound;
		//iter++;
	}
}   

void Game::setMusicVolume(int volume)
{
	this->musicVolume = volume;
	Mix_VolumeMusic(this->musicVolume);
}

void Game::setSoundVolume(int volume)
{
	this->soundVolume = volume;
	Mix_Volume(-1, this->soundVolume);
}

int Game::keyHeldDown(int keysym)
{
	Uint8 *keyState = NULL;
	int numKeys = 0;
	
	keyState = SDL_GetKeyState(&numKeys);
	if ( this->lastKeyState == NULL ) {
		return 0;
	}
	if ( keysym < this->lastNumKeys && keysym < numKeys ) {
		return 0;
	}
	if ( this->lastKeyState[keysym] + keyState[keysym] == 2 ) {
		return 1;
	}
	return 0;
}

// ------------ class factory and memory management stuff

SpriteStrip *Game::newSpriteStrip(std::string key)
{
	SpriteStrip *obj = new SpriteStrip();
	if ( obj == NULL )
		return NULL; 
	if ( key != "" ) {
		if ( this->addSpriteStrip(obj, key) != 0 ) {
			delete obj;
			return NULL;
		}
	}
	return obj;
}

int Game::freeSpriteStrip(std::string key, SpriteStrip *obj)
{ 
	if ( obj == NULL && this->spriteMap.count(key) == 0 ) {
		return 1;
	} else if ( obj == NULL ) {
		obj = this->spriteMap[key];
	}
	this->spriteMap.erase(key);
	delete obj;
}

Animation *Game::newAnimation(std::string key)
{
	Animation *obj = new Animation();
	if ( obj == NULL )
		return NULL; 
	if ( key != "" ) {
		if ( this->addAnimation(obj, key) != 0 ) {
			delete obj;
			return NULL;
		}
	}
	return obj;
}

int Game::freeAnimation(std::string key, Animation *obj)
{ 
	if ( obj == NULL && this->animationMap.count(key) == 0 ) {
		return 1;
	} else if ( obj == NULL ) {
		obj = this->animationMap[key];
	}
	this->animationMap.erase(key);
	delete obj;
}

Actor *Game::newActor(std::string key)
{
	Actor *obj = new Actor();
	if ( obj == NULL )
		return NULL; 
	if ( key != "" ) {
		if ( this->addActor(obj, key) != 0 ) {
			delete obj;
			return NULL;
		}
	}
	return obj;
}

int Game::freeActor(std::string key, Actor *obj)
{ 
	if ( obj == NULL && this->actorMap.count(key) == 0 ) {
		return 1;
	} else if ( obj == NULL ) {
		obj = this->actorMap[key];
	}
	this->actorMap.erase(key);
	delete obj;
}


Display2D *Game::newDisplay2D(std::string key)
{
	Display2D *obj = new Display2D();
	if ( obj == NULL )
		return NULL; 
	if ( key != "" ) {
		if ( this->addDisplay(obj, key) != 0 ) {
			delete obj;
			return NULL;
		}
	}
	return obj;
}

int Game::freeDisplay2D(std::string key, Display2D *obj)
{ 
	if ( obj == NULL && this->displayMap.count(key) == 0 ) {
		return 1;
	} else if ( obj == NULL ) {
		obj = (Display2D *)this->displayMap[key];
	}
	this->displayMap.erase(key);
	delete obj;
}

MenuDisplay *Game::newMenuDisplay(std::string key)
{
	MenuDisplay *obj = new MenuDisplay();
	if ( obj == NULL )
		return NULL; 
	if ( key != "" ) {
		if ( this->addDisplay(obj, key) != 0 ) {
			delete obj;
			return NULL;
		}
	}
	return obj;
}

int Game::freeMenuDisplay(std::string key, MenuDisplay *obj)
{ 
	if ( obj == NULL && this->displayMap.count(key) == 0 ) {
		return 1;
	} else if ( obj == NULL ) {
		obj = (MenuDisplay *)this->displayMap[key];
	}
	this->displayMap.erase(key);
	delete obj;
}

int Game::addSpriteStrip(SpriteStrip *strip, std::string key)
{
	if ( strip == NULL || this->spriteMap.count(key) > 0 )
		return 1;
	this->spriteMap[key] = strip;
	return 0;
}

SpriteStrip *Game::getSpriteStrip(std::string key)
{
	if ( this->spriteMap.count(key) > 0 ) {
		return this->spriteMap[key];
	}
	return NULL;
}

int Game::removeSpriteStrip(std::string stripName)
{
	if ( this->spriteMap.count(stripName) > 0 ) {
		this->spriteMap.erase(stripName);
		return 0;
	}
	return 1;
}

int Game::addAnimation(Animation *ptr, std::string key)
{
	if ( ptr != NULL || this->animationMap.count(key) == 0 ) {
		this->animationMap[key] = ptr;
		return 0;
	}
	return 1;
}

Animation *Game::getAnimation(std::string key)
{
	if ( this->animationMap.count(key) > 0 ) {
		return this->animationMap[key];
	}
	return NULL;
}

int Game::removeAnimation(std::string key)
{
	if ( this->animationMap.count(key) > 0 ) {
		this->animationMap.erase(key);
		return 0;
	}
	return 1;
}

int Game::addActor(Actor *ptr, std::string key)
{
	if ( ptr != NULL || this->actorMap.count(key) == 0 ) {
		this->actorMap[key] = ptr;
		return 0;
	}
	return 1;
}

Actor *Game::getActor(std::string key)
{
	if ( this->actorMap.count(key) > 0 ) {
		return this->actorMap[key];
	}
	return NULL;
}

int Game::removeActor(std::string key)
{
	if ( this->actorMap.count(key) > 0 ) {
		this->actorMap.erase(key);
		return 0;
	}
	return 1;
}

int Game::addDisplay(Display *ptr, std::string key)
{
	if ( ptr != NULL || this->displayMap.count(key) == 0 ) {
		this->displayMap[key] = ptr;
		return 0;
	}
	return 1;
}

Display *Game::getDisplay(std::string key)
{
	if ( this->displayMap.count(key) > 0 ) {
		return this->displayMap[key];
	}
	return NULL;
}

int Game::removeDisplay(std::string key)
{
	if ( this->displayMap.count(key) > 0 ) {
		this->displayMap.erase(key);
		return 0;
	}
	return 1;
}


void Game::lockFPS(int fps)
{
	this->fpslock = fps;
}

void Game::finishFrame()
{
	SDL_Flip(this->canvas);
}

SDL_Surface *Game::getCanvas()
{
	return this->canvas;
}
