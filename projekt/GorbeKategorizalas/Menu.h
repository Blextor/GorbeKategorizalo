#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "common.h"
#include "button.h"
#include "adatBeolvas.h"
#include "adatLetoltes.h"
#include "stock.h"
#include "config.h"

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
    Menu *gorbeMenu;

    Text FoCim;

    Button FrissMB, RCSMB;
    Button GorbeMB, ElemzesMB;
    Button KilepB;

    Button szalMennyisegB;
    Text szalakKiv;
    Text szalakKivInfo;

    void gombokKialakitasa();

    FoMenu(){}
    FoMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *frissito, Menu *csoport, Menu *gorbe);

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

    Text kivalasztottDologT; /// kiválaszott részvényt vagy csoportot jelző szöveg
    ProgressBar progBar; /// töltőcsík
    Button PSSB, PCB; /// progressBar start/stop gomb és cancel gomb

    bool letoltesValasztva = false;
    string kivResz = "", kivCsop = "";

    vector<string> meglevoReszvenyek;
    vector<string> meglevoCsoportok;

    thread letoltoSzal;
    bool stopped = true;
    bool inProc = false;

    bool startStop(){
        if (!inProc){
            if (letoltoSzal.joinable()){
                letoltoSzal.join();
            }
            if (adatokLetoltese()){
                stopped=false;
                inProc=true;
                PSSB.str="stop";
                progBar.start();
            } else {
                kivalasztottDologT.str="Nincs kivalasztva reszveny vagy csoport!";
            }
        } else {
            if (stopped){
                progBar.start();
                stopped=false;
                PSSB.str="stop";
            } else {
                progBar.stop();
                stopped=true;
                PSSB.str="start";
            }

        }
        return false;
    }

    bool cancel(){
        progBar.cancel();
        if (!inProc){
            if (kivCsop!="" || kivResz!=""){
                kivCsop="";
                kivResz="";
                kivalasztottDologT.str="Torolve a kivalasztott!";
                stopped=true;
            }
        } else {
            stopped=true;
            PSSB.str="start";
            inProc=false;
            kivalasztottDologT.str="Megszakitva!";
            kivCsop="";
            kivResz="";
        }
        return false;
    }

    void kivReszSet(string str){
        cancel();
        transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return toupper(c); });
        kivResz=str;
        kivCsop="";
    }

    void kivCsopSet(string str){
        cancel();
        kivCsop=str;
        kivResz="";
    }

    void gombokKialakitasa();

    void ujElemFeldolgozva(int x){
        progBar.elemFeldolgozva(x);
    }

    int elemszamKiszamolasa(int reszvenyekSzama=1){
        int y = getActYear();
        int m = getActMonth();
        int Y=2000, M=1;
        int monthCnt = (y-2000)*12+m;
        //cout<<y<<" "<<m<<" "<<Y<<" "<<M<<" "<<monthCnt<<endl;
        return (1+2+monthCnt)*reszvenyekSzama;
    }

    bool adatokLetoltese(){
        int x=0;
        if (kivResz!=""){
            if (letoltoSzal.joinable()){
                cout<<"JoinBajAdatLetol"<<endl;
                letoltoSzal.join();
            }
            if (letoltesValasztva) {
                progBar.prepare(elemszamKiszamolasa());
                progBar.start();
                letoltoSzal = move(thread(reszvenyAPILetoltes,ref(kivResz),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));// reszvenyAPILetoltes(kivResz);
            }
            else {
                progBar.prepare(elemszamKiszamolasa());
                progBar.start();
                letoltoSzal = move(thread(reszvenyAPIFrissites,ref(kivResz),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));
            }
        }
        else if (kivCsop!=""){
            if (letoltoSzal.joinable()){
                letoltoSzal.join();
            }
            if (letoltesValasztva){
                progBar.prepare(elemszamKiszamolasa(csoportReszvenyei(kivCsop).size()));
                progBar.start();
                letoltoSzal = move(thread(csoportAPILetoltes,ref(kivCsop),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));
            }
            else {
                progBar.prepare(elemszamKiszamolasa(csoportReszvenyei(kivCsop).size()));
                progBar.start();
                letoltoSzal = move(thread(csoportAPIFrissites,ref(kivCsop),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));
            }
        } else {
            return false;
        }
        return true;
    }

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

struct ReszvenyMenu : public Menu {

    Text FoCim; /// fejléc
    Button FoMB, ElmMB, CimMB; /// menü gombok
    Menu *cimkeMenu, *foMenu, *elemzoMenu;

    Button ujReszInp, reszOKB; /// új részvény sor létrehozása
    Gorgetheto reszvenyLista;

    vector<bool> boolR;
    vector<ReszvenySor> reszvenyek; /// a részvények sorai


    bool reszvenyekClick(int bx, int by);

    void gombokKialakitasa();

    ReszvenyMenu(){}
    ReszvenyMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fomenu);

    void draw() override;
    void inputHandle() override;
    void process() override;
};



struct MenuK {
    Menu *startMenu;
    FoMenu fomenu;
    FrissitoMenu frissitomenu;
    CsoportEditorMenu csoportEditormenu;
    ReszvenyMenu reszvenyMenu;

    MenuK(SDL_Pack sdlp, Menu **act);
};


#endif // MENU_H_INCLUDED
