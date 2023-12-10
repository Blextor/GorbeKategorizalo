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
    Menu *gorbeMenu, *elemzoMenu;

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

    void nextMenus(Menu *frissito, Menu *csoport, Menu *gorbe, Menu *elemzes);

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

    bool startStop(){ /// start stop gomb fő függvénye
        if (!inProc){ /// ha nincs folyamatban épp egy letöltés
            if (letoltoSzal.joinable()){ /// a meglévő szálat bevárjuk (biztos ami biztos)
                letoltoSzal.join();
            }
            if (adatokLetoltese()){ /// majd ha sikerül az adatokat letölteni
                stopped=false; /// akkor nem áll a folyamat
                inProc=true; /// és van is folyamat
                PSSB.str="stop"; /// a start gomb stop lesz
                progBar.start(); /// és a progressBar elkezdi számolni az időt
            } else { /// ha nem sikerül, akkor nicns kiválasztva részvény
                kivalasztottDologT.str="Nincs kivalasztva reszveny vagy csoport!";
            }
        } else {    /// ha van folyamat
            if (stopped){ /// akkor ha áll folytassuk
                progBar.start(); /// inditsuk újra el az időt számlálót
                stopped=false; /// nem fog tovább állni a folyamat
                PSSB.str="stop"; /// és majd stop-oljuk max
            } else { /// ha nem áll
                progBar.stop(); /// akkor megállítjuk a stoppert
                stopped=true; /// áll a folyamat
                PSSB.str="start"; /// és majd elindítjuk
            }

        }
        return false; /// felesleges
    }

    bool cancel(){ /// megszakítás gomb függvénye
        progBar.cancel(); /// progressBar reseteli magát
        if (!inProc){ /// ha nem volt folyamat
            if (kivCsop!="" || kivResz!=""){ /// akkor max kiválasztva volt csak valami
                kivCsop=""; /// mind a csoportot törlöm
                kivResz=""; /// mind a részvényt is
                kivalasztottDologT.str="Torolve a kivalasztott!"; /// visszajelzést kiírok
                stopped=true; /// és biztos ami biztos megállítom a folyamatot
            }
        } else { /// ha volt folyamatban valami
            stopped=true; /// akkor először álljon meg
            PSSB.str="start"; /// legközeleb biztos el kell indítani majd valamit
            inProc=false; /// a folyamat is álljon le
            kivalasztottDologT.str="Megszakitva!"; /// adjunk visszajelzést
            kivCsop=""; /// és törlöm a kiválasztott csoportot
            kivResz=""; /// és a kiválasztott részvényt is
        }
        return false; /// felesleges
    }

    void kivReszSet(string str){ /// részvény kiválasztása
        cancel(); /// resetel
        transform(str.begin(), str.end(), str.begin(), [](unsigned char c){ return toupper(c); });
        kivResz=str; /// nagybetűsít, illetve beállítja kiválasztottként
        kivCsop=""; /// a csoportot törli
    }

    void kivCsopSet(string str){ /// csoport kiválasztása
        cancel(); /// resetel
        kivCsop=str; /// nagybetűsít, illetve beállítja kiválasztottként
        kivResz=""; /// a részvényt törli
    }

    void gombokKialakitasa(); /// a gombok elrendezését ide szerveztem ki init-ből

    void ujElemFeldolgozva(int x){ /// progressBar-nak egy átadható függvénye
        progBar.elemFeldolgozva(x); /// hogy hány elemet dolgozott fel a szál
    }

    int elemszamKiszamolasa(int reszvenyekSzama=1){ /// becsléshez, hogy hány elem van hátra még
        int y = getActYear();
        int m = getActMonth();
        int Y=2000, M=1;
        char c;
        int monthCnt = (y-2000)*12+m;
        if (letoltesValasztva ) return (1+2+monthCnt)*reszvenyekSzama;
        string path = Config.getRootDirectory() + "stocks\\";
        if (kivResz!="") {
            string reszPath = path+kivResz+"\\months";
            vector<string> fajlok = getFiles(reszPath);
            if (fajlok.size()==0) return (1+2+monthCnt)*reszvenyekSzama;
            stringstream ss; ss<<fajlok[fajlok.size()-1];
            ss>>Y>>c>>M;
            return (y-Y)*12+m-M +1+2+1; /// a különbség (+1 az utolsót újra letöltöm) + 3
        } else {
            int szum=0;
            vector<string> elemek = csoportReszvenyei(kivCsop);
            for (size_t i=0; i<elemek.size();i++){
                string reszPath = path+elemek[i]+"\\months";
                cout<<reszPath<<endl;
                vector<string> fajlok = getFiles(reszPath);
                if (fajlok.size()==0) {szum+=(1+2+monthCnt); continue;}
                stringstream ss; ss<<fajlok[fajlok.size()-1];
                ss>>Y>>c>>M;
                szum+=(y-Y)*12+m-M +1+2+1; /// a különbség (+1 az utolsót újra letöltöm) + 3
                cout<<szum<<endl;
            }
            cout<<"Return :"<<szum<<endl;
            return szum;
        }
        //cout<<y<<" "<<m<<" "<<Y<<" "<<M<<" "<<monthCnt<<endl;
        //return (1+2+monthCnt)*reszvenyekSzama;
    }

    bool adatokLetoltese(){ /// adatok letöltését megkezdi
        if (kivResz!=""){ /// ha van kiválaszott részvény
            if (letoltoSzal.joinable()){ /// létező szálat bevárom
                cout<<"JoinBajAdatLetol"<<endl; /// elvileg ilyen nincs
                letoltoSzal.join();
            }
            if (letoltesValasztva) { /// majd ha letöletni kívánom
                progBar.prepare(elemszamKiszamolasa()); /// akkor a progressBar-t felkészítem
                progBar.start(); /// és el is indítom, ugyanis külön szálban elindítom a letöltést
                letoltoSzal = move(thread(reszvenyAPILetoltes,ref(kivResz),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));// reszvenyAPILetoltes(kivResz);
            }
            else { /// ha csak frissítésről van szó
                progBar.prepare(elemszamKiszamolasa());
                progBar.start(); /// akkor ugyan az történik, másik, a frissítő függvénnyel
                letoltoSzal = move(thread(reszvenyAPIFrissites,ref(kivResz),ref(stopped),ref(inProc),bind(ujElemFeldolgozva,this,std::placeholders::_1)));
            }
        }
        else if (kivCsop!=""){ /// ha csoport van kijelölve, az akkor nagyon hasonló a részvényhez
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
            return false; /// ha nincs se csoport se részvény kiválasztva
        }
        return true; /// egyébként elindult egy szál
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
    UjCimkePopUp ujCimkePopUp;

    vector<bool> boolR;
    vector<ReszvenySor> reszvenyek; /// a részvények sorai

    bool ujCimkePopUpB = false;

    int panX=0, panY=0; /// a vízszintes és függőleges mozgatásért

    bool reszvenyekClick(int bx, int by);

    void gombokKialakitasa();

    ReszvenyMenu(){}
    ReszvenyMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fomenu, Menu *elemzes);

    void draw() override;
    void inputHandle() override;
    void process() override;
};

struct ElemzesMenu : public Menu {

    Text FoCim; /// fejléc
    Button fomenubeB, elemzeshezB; /// menük közötti navigáló gomb
    Menu *foMenu, *elemzesFolyamatMenu; /// vissza a főmenübe, vagy a lekérdezés elindításához

    Button reszInp, reszCsopInp; /// részvény vagy csoport kiválasztása
    Text reszInpT, reszCsopInpT; /// szövegek hozzájuk
    Gorgetheto reszvenyLista, reszvenyCsoportLista; /// ezekhez görgethető felület

    Text tolT; /// dátumtól szöveg
    Button evTol, honapTol, napTol; /// és a három bemenet
    Text igT; /// dátumig szöveg
    Button evIg, honapIg, napIg; /// és a három bemenet

    Button ujNap, ujNapKomp, ujNegyed, ujNegyedKomp; /// hogy milyen címkefajtát kívánunk hozzáadni
    Button ujFeltetel, ujElemezendo; /// hogy feltétel lesz belőle, vagy elemezendő

    Text feltetelT, elemezendoT; /// címkelisták fejléce

    vector<Feltetel> feltetelek, elemezendoek;
    int feltetelPosX = 170, elemezendoPosX = 487;

    bool ujCimkePopUpB = false;

    int panYFelt=0, panYElemez=0; /// a vízszintes és függőleges mozgatásért
    int datumChState = 0, reszState = 0, tipusState = 0; /// melyik gombot nyomtam le

    vector<string> meglevoReszvenyek; /// szelekció
    vector<string> meglevoCsoportok;
    void feltetelAdd(bool elemezendo);

    void gombokKialakitasa();

    ElemzesMenu(){}
    ElemzesMenu(SDL_Pack sdlp,Menu **act) : Menu(sdlp,act){}

    void nextMenus(Menu *fomenu, Menu *elemzeshez);

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
    ElemzesMenu elemzesMenu;

    MenuK(SDL_Pack sdlp, Menu **act);
};


#endif // MENU_H_INCLUDED
