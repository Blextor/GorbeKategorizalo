#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "common.h"
#include "button.h"
#include "adatBeolvas.h"

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
    int state = 0;
    int oldState = 0;
    bool kiszamoltState = false;

    Uint32 timestampText = 0;

    Menu **menu;

    Menu(){}
    Menu(SDL_Pack sdlp, Menu **act);

    virtual void draw();
    virtual void inputHandle();
    virtual void process();
};

struct FoMenu : public Menu {

    Menu *frissitoMenu;
    Menu *csoportEditormenu;

    FoMenu(){}
    FoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *frissito, Menu *csoport);

    void draw() override;
    void inputHandle() override;
    void process() override;
};

struct FrissitoMenu : public Menu {

    Menu *fomenu;
    Menu *csoportEditormenu;

    FrissitoMenu(){}
    FrissitoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fo, Menu *csoport);

    void draw() override;
    void inputHandle() override;
    void process() override;
};

struct CsoportEditorMenu : public Menu {

    Menu *fomenu;
    Menu *frissitomenu;

    Button FMB, AFMB; /// fomenu, adat frissito menu

    Text MegCimTxt; /// Magyarázó cím
    Button ReszMegB, CsoportMegB;   /// uj dolog beviteli mezője
    Button ReszMegPluszB, CsoportMegPluszB; /// mentés gomb
    Text ReszMegTxt, CsoportMegTxt; /// visszajelzés

    Text MinCimTxt; /// Magyarázó cím
    Button ReszMinB, CsoportMinB;   /// törölni kívánt dolog beviteli mezője
    Gorgetheto ReszMinG, CsoportMinG;   /// törölni kívánt dologok listája
    Button ReszMinMinB, CsoportMinMinB; /// törlés gomb
    Text ReszMinTxt, CsoportMinTxt; /// visszajelzés

    bool firstInitOszzLista = true; /// a részvényeket először kirajzolja
    Text CsopValTxt;    ///
    Gorgetheto OsszResz;
    Button AktCsop, AktCsopOk;
    string aktCsopStr = "";
    Gorgetheto OsszCsop;

    vector<string> meglevoReszvenyek;
    vector<string> meglevoCsoportok;
    vector<set<string>> csoportokListaja;

    vector<string> aktualisCsoportListaja;



    void gombokKialakitasa();

    CsoportEditorMenu(){}
    CsoportEditorMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fo, Menu *friss);

    void draw() override;
    void inputHandle() override;
    void process() override;
};






struct MenuK {
    Menu *startMenu;
    FoMenu fomenu;
    FrissitoMenu frissitomenu;
    CsoportEditorMenu csoportEditormenu;

    MenuK(SDL_Pack sdlp, Menu **act);
};


#endif // MENU_H_INCLUDED
