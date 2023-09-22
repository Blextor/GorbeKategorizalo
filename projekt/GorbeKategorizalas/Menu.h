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

    Text FoCim, kisLeiras; /// menü neve + a letöltés és a frissítés közötti különbség

    Button FMB, RCSMB; /// főmenübe és részvény/csoport szerkesztő menübe gomb

    Button ReszBevB; /// részvény beviteli mezője
    Gorgetheto ReszBevG; /// részvények listája
    Button ReszLetB, ReszFrissB; /// részvény letöltésének és frissítésének gombja
    Text ReszTxt, ReszVissz; /// részvény felirat és visszajelzés

    Button CsopBevB; /// csoport beviteli mezője
    Gorgetheto CsopBevG; /// csoport listája
    Button CsopLetB, CsopFrissB; /// csoport letöltésének és frissítésének gombja
    Text CsopTxt, CsopVissz; /// csoport felirat és visszajelzés

    ProgressBar progBar; /// töltőcsík
    Button PSSB, PCB; /// progressBar start/stop gomb és cancel gomb


    vector<string> meglevoReszvenyek;
    vector<string> meglevoCsoportok;

    void gombokKialakitasa();

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

    Text FoCim; /// Menü címe

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
    Text CsopValTxt;    /// Csoport választás feletti szöveg
    Gorgetheto OsszResz;    /// összes részvény helye
    Button AktCsop, AktCsopOk; /// A csoport választás mezője és a jóváhagyó gombja
    string aktCsopStr = ""; /// az elmentett csoport neve
    Gorgetheto OsszCsop; /// a görgethető csoport választó
    Button CsopMent; /// Elmenti a csoport elemeinek változtatását
    Button CsopCsak; /// Csak a csoport (még) elemei láthatóak (törléshez hasznosabb)
    Text CsopMentTxt; /// visszajelzés a mentésről
    Text OsszReszTxt; /// részvények címe

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
