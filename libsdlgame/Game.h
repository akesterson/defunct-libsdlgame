#ifndef __GAME_H__
#define __GAME_H__

#include <SDL.h>
#include "SpriteStrip.h"
#include "Animation.h"
#include "Actor.h"
#include "Display.h"
#include "MenuDisplay.h"
#include <SDL_mixer.h>

struct Collision
{
    Actor *actor1;
    Actor *actor2;
};

struct PlayingSound
{
	int channel;
	Mix_Chunk *sound;
};

typedef std::vector<PlayingSound *> PlayingSoundList;

// TODO : Take parts of the Game class and break them out into a Window class.
// 	  the way that the Game class lets you specify a render window tells me that
//        it should really be its own class, w/ a separate class for game logic

typedef std::vector<Display *> DisplayList;

class Game : public SharedCanvas2D
{
protected:
    Mix_Music *bgmusic;
    // format is [ idx = [{filename, sound}, {filename, sound}] ]
    // .. where idx is a channel number, which gives you a vector of SoundPairs.
    PlayingSoundList playingSounds; 
    int musicVolume;
    int soundVolume;
    int lastNumKeys;
    int fpslock;
    Uint8 *lastKeyState;
    std::map<std::string, SpriteStrip *> spriteMap;
    std::map<std::string, Actor *> actorMap;
    std::map<std::string, Animation *> animationMap;
    std::map<std::string, Display *> displayMap;
    
    Game();
    virtual ~Game();
public:
    // easier this way
    DisplayList windows;
    int realfps;
    
    static Game &NewSingleton()
    {
	static Game theGame;
	return theGame;
    }

    SDL_Surface *getCanvas();
    int initVideo(int w, int h, int depth, int flags);
    int initSDL(int audio_rate = 44100, Uint16 audio_format = AUDIO_S16SYS, int audio_channels = 2, int audio_buffers = 4096);
    // this stuff is removed for now because the control event mapping isn't implemented
    // at all and it was causing problems 
    /*
    int handleEvents();
    int handleControlEvent(SDL_Event *event);
    */
    void setMusicVolume(int volume = 128);
    void setSoundVolume(int volume = 128);
    int playMusic(std::string filename);
    void haltMusic();
    int playSound(std::string filename, int loops = 0);
    void cleanSounds(int force = 0);

    // --------------- class factory stuff -----------------
    SpriteStrip *newSpriteStrip(std::string key = "");
    Animation *newAnimation(std::string key = "");
    Actor *newActor(std::string key = "");
    Display2D *newDisplay2D(std::string key = "");
    MenuDisplay *newMenuDisplay(std::string key = "");
    int freeSpriteStrip(std::string key = "", SpriteStrip *obj = NULL);
    int freeAnimation(std::string key = "", Animation *obj = NULL);
    int freeActor(std::string key = "", Actor *obj = NULL);
    int freeDisplay2D(std::string key = "", Display2D *obj = NULL);
    int freeMenuDisplay(std::string key = "", MenuDisplay *obj = NULL);

    // --------------- object tracking ------------------
    int addSpriteStrip(SpriteStrip *strip, std::string key);
    SpriteStrip *getSpriteStrip(std::string stripName);
    int removeSpriteStrip(std::string stripName);
    int addActor(Actor *ptr, std::string key);
    Actor *getActor(std::string key);
    int removeActor(std::string key);
    int addAnimation(Animation *ptr, std::string key);
    Animation *getAnimation(std::string key);
    int removeAnimation(std::string key);
    int addDisplay(Display *ptr, std::string key);
    Display *getDisplay(std::string key);
    int removeDisplay(std::string key);

    // ----------------- updating crap
    void update(int logicOnly = 0);
    void finishFrame();
    void lockFPS(int fps);

    // ----------------- input helpers
    int keyHeldDown(int keysym);
};

#endif // __GAME_H__
