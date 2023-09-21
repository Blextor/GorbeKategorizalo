#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "common.h"

using namespace std;

struct SDL_Pack{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *ev;

    SDL_Pack() {};

    SDL_Pack(SDL_Window *w, SDL_Renderer *r, SDL_Event *e);
};

struct Menu {
    SDL_Pack SDLPack; /// egyszerûség kedvéért, kicsomagolja egybõl
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event *ev;

    Menu **menu;

    Menu(){}
    Menu(SDL_Pack sdlp, Menu **act);

    virtual void draw();
    virtual void inputHandle();
};

struct FoMenu : public Menu {

    Menu *frissitoMenu;

    FoMenu(){}
    FoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *frissito);

    void draw() override;
    void inputHandle() override;
};

struct FrissitoMenu : public Menu {

    Menu *fomenu;

    FrissitoMenu(){}
    FrissitoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fo);

    void draw() override;
    void inputHandle() override;
};






struct MenuK {
    Menu *startMenu;
    FoMenu fomenu;
    FrissitoMenu frissitomenu;

    MenuK(SDL_Pack sdlp, Menu **act);
};


#endif // MENU_H_INCLUDED
