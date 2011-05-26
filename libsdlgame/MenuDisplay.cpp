#include "libsdlgame.h"
#include <iostream>
#include <SDL_ttf.h>
#include <SDL_gfxPrimitives.h>
#include <cstdlib>

// TODO : Need to get the up/down left/right arrows displaying

// ------------------ Menu Relation --------------

MenuRelation::MenuRelation()
{
	this->opt1 = 0;
	this->opt2 = 0;
	this->relation = 0;
}

// ------------------ Menu Option -------------------

MenuOption::MenuOption(std::string name, int rangeLow, int rangeHigh, int rangeStart, int spinFree, SDL_Surface *img)
{
	this->name = name;
	this->rangeLow = rangeLow;
	this->rangeHigh = rangeHigh;
	this->rangeStart = rangeStart;
	this->optionImage = img;
	this->spinFree = spinFree;
	if ( rangeHigh + rangeLow != 0 ) {
		this->selected = rangeStart;
	} else 
		this->selected = -1;
}

MenuOption::~MenuOption()
{
}

void MenuOption::clampValue(MenuRelation *relation, int defClampVal)
{
  // check our relation for clamping issues
  if ( relation != NULL && relation->opt1 != NULL && relation->opt2 != NULL && relation->opt1 == this ) {
    std::cerr << "[" << this << "]->selected (" << this->selected << "=" << this->getStringValue() << ") = [" << relation->opt2 << "]->selected (" << relation->opt2->selected << "=" << relation->opt2->getStringValue() << ")\n";
    if ( (relation->relation == MENURELATION_NODUPLICATE) && 
	 (this->getStringValue() == relation->opt2->getStringValue()) ) {
      if ( ((this->selected+1) >= (int) this->optlist.size()) ||
	   ((this->rangeHigh != 0) && (this->selected+1 > this->rangeHigh)) ){
	//std::cerr << "Clamping DOWN\n";
	this->selected -= 1;
      } else if ( ((this->selected - 1) < 0) ||
		  ((this->rangeLow != 0) && (this->selected - 1 < this->rangeLow)) ) {
	//std::cerr << "Clamping UP\n";
	this->selected += 1;
      } else {
	// we're safe to move either direction on the option here, so just ++ it and then clamp it
	this->selected += defClampVal;
      }
      this->clampValue(NULL);
      //std::cerr << "[" << this << "]->selected (" << this->selected << "=" << this->getStringValue() << ") = [" << relation->opt2 << "]->selected (" << relation->opt2->selected << "=" << relation->opt2->getStringValue() << ")\n";
    }
  }
  else {
    if ( this->optlist.size() > 0 ) {
      if ( this->selected >= (int) this->optlist.size() ) {
	this->selected = this->optlist.size()-1;
      } else if ( this->selected < 0 ) {
	this->selected = 0;
      }
    } else {
      if ( this->selected < this->rangeLow ) {
	this->selected = this->rangeLow;
      } else if ( this->selected > this->rangeHigh ) {
	this->selected = this->rangeHigh;
      } else if ( this->rangeLow > this->rangeHigh || this->rangeLow + this->rangeHigh == 0 ) {
	this->selected = 0;
      }
    }
  }
  //std::cerr << "MenuOption::clampValue " << this->name 
  //<< " (" << this->optlist.size() << " sub-options ) "
  //<< " range (" << this->rangeLow << " - " << this->rangeHigh << ") "
  //<< " selected " << this->selected << "\n";
}

std::string MenuOption::getStringValue()
{
	char intStringBuff[64];
	memset((char *)&intStringBuff, 0x00, 63);
	if ( this->rangeLow + this->rangeHigh != 0 ) {
		sprintf((char *)&intStringBuff, "%d\0", this->selected); 
		return std::string((char *)&intStringBuff);
	}
	if ( this->optlist.size() > this->selected )
		return this->optlist.at(this->selected);
	else
		return this->name;
}

int MenuOption::getIntValue()
{
	return this->selected;
}

int MenuOption::select(int value)
{
  if ( ( this->optlist.size() > 0 ) && ( value < this->optlist.size()) && ( value > -1 ) ) {
    this->selected = value;
    return 0;
  } else if ( (value >= this->rangeLow) && ( value <= this->rangeHigh ) ) {
    this->selected = value;
    return 0;
  }
  return 1;
}

int MenuOption::whenhighlighted() 
{
  return 0;
}

int MenuOption::whenselected()
{
  return 0;
}

int MenuOption::valueup()
{
  this->select(this->selected + 1);
}

int MenuOption::valuedown()
{
  this->select(this->selected - 1);
}

// ----------------- MenuDisplay -------------------- 

MenuDisplay::MenuDisplay()
{
	this->pointerActor = NULL;
	this->curOpt = 0;
	this->origin.x = 0;
	this->origin.y = 0;
	this->origin.z = 0;
	this->fontname = "";
	this->pointsize = 0; 
	this->spacing = 0;
	this->closeOpt = "";
}

MenuDisplay::~MenuDisplay()
{
	if ( this->pointerActor != NULL ) {
		delete this->pointerActor;
	}
}

void MenuDisplay::setMenuImages(Animation *arrowLeft, Animation *arrowRight, Animation *arrowUp, Animation *arrowDown)
{
	this->arrowLeft = arrowLeft;
	this->arrowRight = arrowRight;
	this->arrowUp = arrowUp;
	this->arrowDown = arrowDown;
}

// this is just for backwards compatibility with old code.
void MenuDisplay::addOption(std::string name, int rangeLow = 0, int rangeHigh = 0, int rangeStart = 0, int spinFree = 0, SDL_Surface *img = NULL)
{
  this->addOption(new MenuOption(name, rangeLow, rangeHigh, rangeStart, spinFree, img) );
}

void MenuDisplay::addOption(MenuOption *mopt)
{
  if ( mopt != NULL )
    this->menuOptions.push_back(mopt);
}

int MenuDisplay::addSubOption(std::string name, std::string subopt)
{
	std::vector<MenuOption *>::iterator iter;
	iter = this->menuOptions.begin();
	while ( iter != this->menuOptions.end() ) {
		if ( *iter != NULL && (*iter)->optionImage != NULL ) {
			// there's zero point in adding a sub option string when this has an image
			return 1;
		}
		if ( *iter != NULL && (*iter)->name == name ) {
			(*iter)->optlist.push_back(subopt);
			(*iter)->select(0);
			return 0;
		}
		iter++;
	}
	return 1;
}

int MenuDisplay::setPointer(Animation *ptr)
{  
	if ( ptr == NULL ) {
		//std::cerr << "MenuDisplay::setPointer was passed a NULL Animation.\n";
		return 1;
	}
	if ( this->pointerActor != NULL ) {
		delete this->pointerActor;
	} else {
		this->pointerActor = new Actor();
		if ( this->pointerActor == NULL ) {
			//std::cerr << "Couldn't allocate memory for new pointer actor\n";
			return 1;
		}
	}
	//std::cerr << "Set animation " << ptr << " as animation for actor " << this->pointerActor << " for menu pointer\n";
	this->pointerActor->addAnimation(ptr, STATE_NONE);
	this->pointerActor->addState(STATE_NONE);
	return 0;
}

MenuOption *MenuDisplay::getOption(std::string name)
{
	std::vector<MenuOption *>::iterator menuIter;
	MenuOption *opt = NULL;
	menuIter = this->menuOptions.begin();
	if ( name != "" ) {
		while ( menuIter != this->menuOptions.end() ) {
			opt = *menuIter;
			//std::cerr << "Checking " << opt->name << " against " << name << "\n";
			if ( opt->name == name ) {
				return opt;
			}
			menuIter++;
		}
	} else {
		//std::cerr << "Returning option at " << this->curOpt << "\n";
		return this->menuOptions.at(this->curOpt);
	}
	return NULL;
}       

void MenuDisplay::setMenuOrigin(Vector origin)
{
	this->origin.x = origin.x;
	this->origin.y = origin.y;
	this->origin.z = origin.z;
	//std::cerr << "MenuDisplay::setMenuOrigin x = " << this->origin.x << " y = " << this->origin.y << " z = " << this->origin.z << "\n";
}

void MenuDisplay::update(int logicOnly)
{
	SDL_Surface *frame = NULL;
	SDL_Rect destrect;
	SDL_Rect lastdirty;
	MenuOption *opt = NULL;
	FontRenderer &textEngine = FontRenderer::NewSingleton();
	int lastwidth = 0;
	std::vector<int> yvalues;

	if ( this->active == 0 ) {
		return;
	}
	Display2D::update(logicOnly);


	//std::cerr << "curOpt now " << this->curOpt << "\n";
	if ( this->curOpt >= (int) this->menuOptions.size() ) {
		this->curOpt = this->menuOptions.size() - 1;
	} else if ( this->curOpt < 0 ) {
		this->curOpt = 0;
	}

	if ( logicOnly == 1 ) {
		return;
	}
	//std::cerr << "fixed curOpt now " << this->curOpt << "\n";

	lastdirty.x = (Sint16) this->origin.x;
	lastdirty.y = (Sint16) this->origin.y;
	lastdirty.w = 0;
	lastdirty.h = 0;

	frame = this->pointerActor->nextFrame();

	if ( frame != NULL ) {
		lastdirty.x += (frame->w + 10);
	}

	for ( unsigned int i = 0; i < this->menuOptions.size(); i++ ) {
		opt = this->menuOptions.at(i);
		if ( opt == NULL ) {
			continue;
		}
		//std::cerr << " in MenuDisplay::update i = " << i << " curOpt = " << curOpt << "\n";
		//std::cerr << "String value of currently rendering option: " << opt->getStringValue() << "\n";
		lastdirty = textEngine.renderString(opt->name, this->canvas, this->fontname, (Vector){lastdirty.x, lastdirty.y + lastdirty.h, 0}, this->pointsize);
		if ( i == (unsigned int) curOpt ) {
			//std::cerr << this->optHeight * this->curOpt << " = " << this->origin.y + (this->optHeight * this->curOpt) << " = " << ptrdest.y << "\n";
			//std::cerr << "MenuDisplay origin is x = " << this->origin.x << " y = " << this->origin.y << " z = " << this->origin.z << " optHeight is " << this->optHeight << " curOpt is " << this->curOpt << "\n";
			if ( frame != NULL ) {
				// the pointer actually appears slightly to the left of the current option
				destrect.x = (Sint16) this->origin.x;
				destrect.y = lastdirty.y + ((lastdirty.h/2) - frame->h/2);
				destrect.w = 0;
				destrect.h = 0;
				//std::cerr << "Blitting frame " << frame << " to canvas " << this->canvas << " at (" << destrect.x << " x " << destrect.y << ") as pointer.\n";
				SDL_BlitSurface(frame, NULL, this->canvas, &destrect);
			}
		}
		lastdirty.y += this->spacing;
		if ( lastdirty.w > lastwidth ) {
			lastwidth = lastdirty.w;
		}
		yvalues.push_back(lastdirty.y - this->spacing);
	}
	for ( unsigned int i = 0; i < this->menuOptions.size(); i++ ) {
		opt = this->menuOptions.at(i);
		if ( opt->getStringValue() != opt->name ) {
			textEngine.renderString(opt->getStringValue(), this->canvas, this->fontname, (Vector){lastdirty.x + lastwidth + 10, yvalues.at(i), 0}, this->pointsize);
		}
	}
	return;
}

void MenuDisplay::setFont(std::string fontname, int pointsize, SDL_Color color, SDL_Color bgcolor)
{
	FontRenderer &textEngine = FontRenderer::NewSingleton(); 
	textEngine.setColor(color, bgcolor, 1);
	this->fontname = fontname;
	this->pointsize = pointsize;
}

int MenuDisplay::handleEvent(SDL_Event *event)
{
  MenuOption *opt = NULL;
  opt = this->getOption(); 
  int retval = 1;
  if ( this->active == 0 || opt == NULL ) {
    return retval;
  }
  //std::cerr << "MenuDisplay::handleEvent\n";
  //switch (event->type) {
  //default:
  if ( event->key.state == SDL_RELEASED && event->key.keysym.sym == SDLK_DOWN ) {
    this->curOpt += 1;   
    if ( this->curOpt < 0 ) {
      this->curOpt = 0;
    }
    retval = 0;
  } else if ( event->key.state == SDL_RELEASED && event->key.keysym.sym == SDLK_UP ) {
    this->curOpt -= 1;   
    if ( this->curOpt >= (int) this->menuOptions.size() ) {
      this->curOpt = this->menuOptions.size()-1;
    }
    retval = 0;
  } else if ( ( event->key.state == SDL_RELEASED || opt->spinFree == 1 && event->key.state == SDL_PRESSED ) 
	      && event->key.keysym.sym == SDLK_LEFT ) {
    opt->valuedown();
    this->clampValue(opt, -1);
    retval = 0;
  } else if ( ( event->key.state == SDL_RELEASED || opt->spinFree == 1 && event->key.state == SDL_PRESSED ) 
	      && event->key.keysym.sym == SDLK_RIGHT ) {
    opt->valueup();
    this->clampValue(opt, 1);
    retval = 0;
  } else if ( event->key.state == SDL_RELEASED && event->key.keysym.sym == SDLK_RETURN ) {
    if ( (this->closeOpt != "") && (opt->name != this->closeOpt)) {
      retval = 0;
    } else if ( ((this->closeOpt != "" ) && (opt->name == this->closeOpt)) || this->closeOpt == "") {
      this->active = 0;
      retval = 0;
    }
  }
  //}
  //opt->clampValue();
  return retval;
}

void MenuDisplay::setCloseOption(std::string option)
{
	this->closeOpt = option;
}

void MenuDisplay::setSpacing(int spacing)
{
	this->spacing = spacing;
}

int MenuDisplay::setRelation(std::string opt1, std::string opt2, int relationType)
{
  std::vector<MenuRelation *>::iterator iter;
  MenuRelation *tmp;
  iter = this->menuRelations.begin();
  while ( iter != this->menuRelations.end() ) {
    tmp = *iter;
    if ( tmp == NULL || tmp->opt1 == NULL || tmp->opt2 == NULL ) {
      iter++;
      continue;
    }
    if ( tmp->opt1->name == opt1 && tmp->opt2->name == opt2 && tmp->relation == relationType ) {
      return 1;
    }  
    iter++;
  }
  tmp = new MenuRelation();
  tmp->opt1 = this->getOption(opt1);
  tmp->opt2 = this->getOption(opt2);
  tmp->relation = relationType;
  this->menuRelations.push_back(tmp);
  this->clampValue(tmp->opt1, 1);
  return 0;
}

void MenuDisplay::clampValue(MenuOption *opt, int defclamp)
{
  std::vector<MenuRelation *>::iterator iter;
  MenuRelation *tmp;
  opt->clampValue();
  //std::cerr << "(PRE-RELATION CLAMP) Option " << opt->name << " now set to option " << opt->getIntValue() << "=" << opt->getStringValue() << "\n";
  iter = this->menuRelations.begin();
  while ( iter != this->menuRelations.end() ) {
    tmp = *iter;
    if ( tmp == NULL || tmp->opt1 == NULL || tmp->opt2 == NULL ) {
      iter++;
      continue;
    }
    if ( tmp->opt1->name == opt->name ) {
      //std::cerr << "Found relation for " << opt->name << " (" << tmp->opt1 << " = " << tmp->opt2 << ")\n";
      opt->clampValue(tmp, defclamp);
    }  
    iter++;
  }
  //std::cerr << "(POST-RELATION CLAMP) Option " << opt->name << " now set to option " << opt->getIntValue << "=" <<opt->getStringValue() << "\n";
}
