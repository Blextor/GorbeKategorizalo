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

    int valtozasiIdx = 0; /// ez indikálja, ha valamelyik menü lemaradásban van
                        /// ha különbség lép fel, akkor dolgoznia kell

    Menu **menu;

    Menu(){}
    Menu(SDL_Pack sdlp, Menu **act);

    virtual void draw();
    virtual void inputHandle();
};

struct FoMenu : public Menu {

    Menu *frissitoMenu;
    Menu *csoportEditormenu;

    FoMenu(){}
    FoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *frissito, Menu *csoport);

    void draw() override;
    void inputHandle() override;
};

struct FrissitoMenu : public Menu {

    Menu *fomenu;
    Menu *csoportEditormenu;

    FrissitoMenu(){}
    FrissitoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fo, Menu *csoport);

    void draw() override;
    void inputHandle() override;
};

struct CsoportEditorMenu : public Menu {

    Menu *fomenu;
    Menu *frissitomenu;

    vector<string> meglevoReszvenyek;
    vector<string> meglevoKategoriak;
    vector<set<string>> kategoriakListaja;

    vector<string> aktualisKategoriaListaja;


    CsoportEditorMenu(){}
    CsoportEditorMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fo, Menu *friss);

    void draw() override;
    void inputHandle() override;
};






struct MenuK {
    Menu *startMenu;
    FoMenu fomenu;
    FrissitoMenu frissitomenu;
    CsoportEditorMenu csoportEditormenu;

    MenuK(SDL_Pack sdlp, Menu **act);
};


#endif // MENU_H_INCLUDED
