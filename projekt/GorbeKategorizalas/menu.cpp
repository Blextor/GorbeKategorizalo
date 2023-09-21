#include "Menu.h"

/// SDL_Pack

SDL_Pack::SDL_Pack(SDL_Window *w, SDL_Renderer *r, SDL_Event *e){window=w; renderer=r; ev=e;}

/// Menu

Menu::Menu(SDL_Pack sdlp, Menu **act) {window=sdlp.window; renderer=sdlp.renderer; ev=sdlp.ev; menu=act;}
void Menu::draw(){};
void Menu::inputHandle(){};
