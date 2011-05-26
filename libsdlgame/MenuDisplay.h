#ifndef __MENUDISPLAY_H__
#define __MENUDISPLAY_H__

#include "Actor.h"
#include "Common.h"
#include "Display2D.h"
#include <vector>

// TODO: Add the ability to set a MenuOption to display as either text or as a slider for numeric values.

typedef struct MenuRelation;

#define MENU_OPTYPE_CHOICE     0
#define MENU_OPTYPE_RANGE      1
#define MENU_OPTYPE_SUBMENU    2

class MenuOption
{
public:
  std::string name;
  SDL_Surface *optionImage;
  std::vector<std::string> optlist;
  int rangeLow;
  int rangeHigh;
  int rangeStart;
  int selected; // doesn't hold info on whether this is selected or not, holds the index of optlist or the range number that is currently selected
  int spinFree; // if this is set to 1, then the value is modified on the presence of a key DOWN event, not a key RELEASE.
  
  MenuOption(std::string name, int rangeLow, int rangeHigh, int rangeStart, int spinFree, SDL_Surface *img);
  ~MenuOption();
  void clampValue(MenuRelation *relation = NULL, int defClampVal = 1);
  int getIntValue();
  std::string getStringValue();	
  virtual int select(int value); // called by MenuDisplay to force selection to a given item
  virtual int whenhighlighted(); // called whenever the menu item is highlighted
  virtual int whenselected(); // called whenever the menu item is selected ("enter" is pressed on the item)
  virtual int valueup(); // called whenever the value of the item is increased (the range is moved up, or the next sub option is selected)
  virtual int valuedown(); // as with valueup(), but works with lower values and previous items
};

#define MENURELATION_NODUPLICATE	0

struct MenuRelation
{
    MenuOption *opt1;
    MenuOption *opt2;
    int relation;
    MenuRelation();
};
 
// class for the menus in the game
// This class dynamically creates actors for the menu options and such
// the only actor you have to feed it is your pointer Animation
class MenuDisplay : public Display2D
{
protected:
	std::vector<MenuOption *> menuOptions; 
	std::vector<MenuRelation *> menuRelations;
	std::vector<MenuDisplay *> subMenus;
	Actor *pointerActor;
	int curOpt;
	Vector origin;
	SDL_Color textColor;
	SDL_Color bgColor;
	std::string fontname;
	int pointsize;
	int spacing;
	std::string closeOpt;
	Animation *arrowUp;
	Animation *arrowDown;
	Animation *arrowLeft;
	Animation *arrowRight;
public: 
	MenuDisplay();
	~MenuDisplay();
	
	void setCloseOption(std::string option);
	void setSpacing(int spacing);
	int setPointer(Animation *ptr);
	void setMenuOrigin(Vector origin);
	void setMenuImages(Animation *arrowLeft, Animation *arrowRight, Animation *arrowUp, Animation *arrowDown);
	void setFont(std::string fontname, int pointsize, SDL_Color color, SDL_Color bgcolor);
	
	void update(int logicOnly = 0);
	int handleEvent(SDL_Event *event);
	void clampValue(MenuOption *opt, int defClamp);
	
	int setRelation(std::string opt1, std::string opt2, int relationType = MENURELATION_NODUPLICATE);
	void setOption(std::string optName, std::string );
	void setOption(std::string optName, int);
	void addOption(MenuOption *mopt);
	void addOption(std::string name, int rangeLow, int rangeHigh, int rangeStart, int spinFree, SDL_Surface *img);
	int addSubOption(std::string name, std::string subopt);
	MenuOption *getOption(std::string name = "");
};

#endif // __MENUDISPLAY_H__
