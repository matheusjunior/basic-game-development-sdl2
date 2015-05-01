#ifndef MENU
#define MENU

#ifdef _WIN32

#include <SDL.h>


#else

#include <SDL2/SDL.h>

#endif

#include <string>

#include "Text.h"

class cMenu {
private:
	bool singleInstance = true;
	Text* m_gameMainMenuOpt1;
	Text* m_gameMainMenuOpt2;
	Text* m_gameMainMenuOpt3;

	int previousSelected = 1;


	SDL_Color colorPause;
	SDL_Color colorSelected;

	const int m_rightArrowX = 34;
	const int m_rightArrowW = 30;

	void updatePosition(Text* t, int px, int py, int w, int h);
public:
	void createElements();
	
	Text static loadFontAndSetPosition(int px, int py, int w, int h);

	Text* loadFontAndSetPositionRef(int px, int py, int w, int h);
	
	void adjustText();

	void updateSelection(int direction);


	Text* getMainMenuOpt1();
	Text* getMainMenuOpt2();
	Text* getMainMenuOpt3();
};


#endif