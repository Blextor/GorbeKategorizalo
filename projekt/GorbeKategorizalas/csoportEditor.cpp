#include "Menu.h"


void CsoportEditorMenu::gombokKialakitasa(){
    /// Menü gombok
    FMB = Button("FoMenube",15,40,70,13,false,true);
    AFMB = Button("Adatok frissitese",110,40,141,13,false,true);
    /// Új elem megadása
    MegCimTxt = Text("Uj reszveny vagy csoport letrehozasa", 20,42);
    ReszMegB = Button("uj reszveny",40,60,124,13,false,false);
    CsoportMegB = Button("uj csoport neve",40,90,124,13,false,false);
    ReszMegPluszB = Button("+",175,60,13,13,false,false);
    CsoportMegPluszB = Button("+",175,90,13,13,false,false);
    ReszMegTxt = Text("", 200,65);
    CsoportMegTxt = Text("", 200,95);
    /// Meglévõ elem törlése
    MinCimTxt = Text("Meglevo reszveny vagy csoport torlese", 330,42);
    ReszMinB = Button("reszveny neve V",350,60,124,13,false,false);
    CsoportMinB = Button("csoport neve  V",350,90,124,13,false,false);
    ReszMinG = Gorgetheto(meglevoReszvenyek,350,75,124,102);
    CsoportMinG = Gorgetheto(meglevoCsoportok,350,105,124,102);
    ReszMinMinB = Button("-",485,60,13,13,false,false);
    CsoportMinMinB = Button("-",485,90,13,13,false,false);
    ReszMinTxt = Text("", 510,65);
    CsoportMinTxt = Text("", 510,95);
    /// Kategória kiválasztása és szerkesztése
    OsszResz = Gorgetheto(meglevoReszvenyek,5,240,400,202,4); OsszResz.elemSzelekt=true;
    AktCsop = Button("",460,240,124,13,false,false);
    AktCsopOk = Button("OK",600,240,20,13,false,false);
    OsszCsop = Gorgetheto(meglevoCsoportok,460,260,124,102);
    CsopValTxt = Text("Valaszott csoport:",456,225);
    CsopMent = Button("Mentes",415,420,60,13,false,false);
    CsopCsak = Button("Az osszes reszveny",450,370,147,13,false,false);
    CsopMentTxt = Text("",488,425);
    OsszReszTxt = Text("Reszvenyek:",10,230);
    /// Menü címe
    FoCim = Text("Reszvenyek es csoportok fejlesztese",10,10);
}

void CsoportEditorMenu::nextMenus(Menu *fo, Menu *friss){
    fomenu=fo;
    frissitomenu=friss;

    gombokKialakitasa(); /// Létrehozza a UI elemeket
}

void CsoportEditorMenu::draw() {

    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255); /// alap szürke
    SDL_RenderClear(renderer);

    FoCim.draw(renderer,x,y);   /// a Menü címe
    lineRGBA(renderer,320,35,320,110,0,0,0,255); /// létrehozás és törlés közti vonal

    /// visszalépés gombok
    if (true){
    FMB.draw(renderer,x,y);
    AFMB.draw(renderer,x,y);
    }

    /// új dolog megadása
    if (true){
    MegCimTxt.draw(renderer,x,y);
    ReszMegB.draw(renderer,x,y); /// bemenetek
    CsoportMegB.draw(renderer,x,y);
    ReszMegPluszB.draw(renderer,x,y);   /// inputok
    CsoportMegPluszB.draw(renderer,x,y);
    ReszMegTxt.draw(renderer,x,y);      /// visszajelzések
    CsoportMegTxt.draw(renderer,x,y);
    }

    /// meglévõ dolog törlése
    if (true){
    MinCimTxt.draw(renderer,x,y);
    if (state==3) ReszMinG.draw(renderer,x,y); /// görgethetõk
    if (state==4) CsoportMinG.draw(renderer,x,y);
    ReszMinB.draw(renderer,x,y); /// bemenetek
    if (state!=3) CsoportMinB.draw(renderer,x,y);
    ReszMinMinB.draw(renderer,x,y);   /// inputok
    CsoportMinMinB.draw(renderer,x,y);
    ReszMinTxt.draw(renderer,x,y);      /// visszajelzések
    CsoportMinTxt.draw(renderer,x,y);
    }

    if (true){  /// Összes részvéby és a csoport kiválasztása + gombok
    OsszResz.draw(renderer,x,y);
    OsszCsop.draw(renderer,x,y);
    AktCsop.draw(renderer,x,y);
    AktCsopOk.draw(renderer,x,y);
    CsopValTxt.draw(renderer,x,y);
    OsszReszTxt.draw(renderer,x,y);
    CsopCsak.draw(renderer,x,y);
    CsopMent.draw(renderer,x,y);
    CsopMentTxt.draw(renderer,x,y);
    }


    SDL_RenderPresent(renderer); /// Kirajzolás ténylegesen
}

void CsoportEditorMenu::inputHandle() {
    int MX=-1, MY=-1; /// kurzor pozíciója, ha -1 marad, nem történt változás
    //bool leftButton = true; /// külön kígyűjtöm, hogy lenyomták-e a bal egérgombot
    bool keyDown = false; /// vagy bármelyt a billentyűzeten
    bool mouseWheel = false; /// vagy görgettek-e

    if (SDL_PollEvent(ev)){ /// lekérem az eseményt és kigyűjtöm, hogy mi történt
        if (ev->type==SDL_MOUSEBUTTONDOWN){ /// csak kattintáskor kérem le az egér pozíciót
            MX=ev->button.x;
            MY=ev->button.y;
            //leftButton=ev->button.button==SDL_BUTTON_LEFT; /// bal gomb
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        }
        if (ev->type==SDL_MOUSEWHEEL){ /// görgtés
            mouseWheel=true;
        }
        if (ev->type==SDL_KEYDOWN){ /// billentyű
            keyDown=true;
            //if (ev->key.keysym.sym==SDLK_r)
        }
        if (ev->type == SDL_QUIT) /// beégetett kilépés engedélyezése
            exit(3);
    }

    if (MX!=-1){ /// azaz az egérrel kattintottunk
        /// kérdés, hogy eltaláltunk-e valami kattinthatót
        if (FMB.inClick(MX,MY)) *menu = fomenu; /// főmenübe írányító gombot
        else if (AFMB.inClick(MX,MY)) *menu = frissitomenu; /// adatokat frissítő menü gombot
        else if (ReszMegB.inClick(MX,MY)) { /// új részvény beviteli mező gomját
            ReszMegB.str="";    /// törlöm a tartalmát
            ReszMegTxt.str="";  /// és törlöm a visszajelzést is
            state = 1;          /// ennek az állapota 1
        }
        else if (CsoportMegB.inClick(MX,MY)) { /// új csoport -||-
            CsoportMegB.str=""; /// ua. előző
            CsoportMegTxt.str="";
            state = 2;          /// ennek 2-es
        }
        else if (ReszMegPluszB.inClick(MX,MY)) { /// új részvény OK gombját
            if (state==1){  /// csak ha előtte legalább belekattintottunk a beviteli mezőbe
                /// lefolyás és visszajelzés
                if (ReszMegB.str.size()==0) ReszMegTxt.str = "Ures!";
                else ReszMegTxt.str = createNewReszveny(ReszMegB.str);
            }
            /// reset eredetibe
            ReszMegB.str="uj reszveny";
            state = 0;
        }
        else if (CsoportMegPluszB.inClick(MX,MY)) { /// új csoport OK gombját
            /// ua. mint az előző
            if (state==2){
                if (CsoportMegB.str.size()==0) CsoportMegTxt.str = "Ures!";
                else CsoportMegTxt.str = createNewCsoport(CsoportMegB.str);
            }
            CsoportMegB.str="uj csoport neve";
            state = 0;
        }
        else if (ReszMinB.inClick(MX,MY)){  /// részvény törlés bevitel
            ReszMinTxt.str="";  /// visszajelzés törlése
            if (state!=3){ /// ha nem nyomtunk még bele
                ReszMinB.str="";    /// törli a szöveget
                ReszMinG.elemekFrissitese(meglevoReszvenyek); /// és lekérdezi a részvényeket
            }
            state=3;    /// ennek az állapota 3-as
        }
        else if (state == 3 && ReszMinG.inClick(MX,MY)){ /// részvény lista
            /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
            string btStr = ReszMinG.whichButton(MX,MY);
            if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                ReszMinB.str=btStr;
                ReszMinG.elemekFrissitese(meglevoReszvenyek,btStr);
            } else {/// csúszka használatra fenntartva

            }
        }
        else if (state == 3 && ReszMinMinB.inClick(MX,MY)){ /// részvény törlés gomb
            state=0;    /// visszajelzés és lefolyás
            if (ReszMinB.str.size()==0){
                ReszMinTxt.str="Ures!";
            } else
                ReszMinTxt.str=deleteReszveny(ReszMinB.str);
        }
        else if (CsoportMinB.inClick(MX,MY)){ /// ua. előző (csoportokra)
            CsoportMinTxt.str="";
            if (state!=4){
                CsoportMinB.str="";
                CsoportMinG.elemekFrissitese(meglevoCsoportok);
            }
            state=4;
        }
        else if (state == 4 && CsoportMinG.inClick(MX,MY)){ /// ua. előző
            string btStr = CsoportMinG.whichButton(MX,MY);
            if (btStr!=""){
                CsoportMinB.str=btStr;
                CsoportMinG.elemekFrissitese(meglevoCsoportok,btStr);
            } else {

            }
        }
        else if (state == 4 && CsoportMinMinB.inClick(MX,MY)){ /// ua. előző
            state=0;
            if (CsoportMinB.str.size()==0){
                CsoportMinTxt.str="Ures!";
            } else
                CsoportMinTxt.str=deleteCsoport(CsoportMinB.str);
        }
        else if (AktCsop.inClick(MX,MY)){ /// csoport kiválasztásának beviteli mezője
            if (state!=5)   /// elég egyszer brefissíteni a csoportokat
                OsszCsop.elemekFrissitese(meglevoCsoportok,"");
            state = 5;
        }
        else if (state == 5 && OsszCsop.inClick(MX,MY)){ /// csoport választó görgő
            string btStr = OsszCsop.whichButton(MX,MY);
            if (btStr!=""){ /// melyik, gomb, ha siker, akkor lefolyás és visszajelzés
                AktCsop.str=btStr;
                OsszCsop.elemekFrissitese(meglevoCsoportok,btStr);
            } else { /// csúszka használatra fenntartva

            }
        }
        else if (state == 5 && AktCsopOk.inClick(MX,MY)){ /// csoport választás jóváhagyás
            CsopMentTxt.str=""; /// előző csoport szerkesztés visszajelzésének törlése
            if (elemeAzStr(meglevoCsoportok,AktCsop.str)){ /// ha valid a csoportnév
                aktCsopStr=AktCsop.str; /// akkor elmenti külön is
                aktualisCsoportListaja=csoportReszvenyei(aktCsopStr); /// befrissíti a csoport részvényeit
                OsszResz.megfeleloElemek=aktualisCsoportListaja; /// és el is tárolja azokat külön
            } else { /// akkor biztos törölni kívántuk
                aktCsopStr="";
                OsszResz.megfeleloElemek.clear();
                aktualisCsoportListaja.clear();
            }
            /// részvénykeet nem árt befrissíteni
            OsszResz.elemekFrissitese(meglevoReszvenyek);
            state = 0;
        }
        else if (CsopCsak.inClick(MX,MY)){ /// a csoport szévényei / összes részvény gomb
            OsszResz.megElem = !OsszResz.megElem; /// negálja és eszerint szövegét megváltoztatja
            if (!OsszResz.megElem) CsopCsak.str="Az osszes reszveny";
            else CsopCsak.str="Csoport reszvenyei";
            OsszResz.elemekFrissitese(meglevoReszvenyek); /// elemeket a változás miatt frissíti
        }
        else if (OsszResz.inClick(MX,MY)){ /// összes részvény görgő
            CsopMentTxt.str=""; /// ha ide kattintunk, nem kell az előző mentés visszajelzés
            string temp = OsszResz.whichButton(MX,MY);
            if (temp!=""){ /// ha valamelyik részvényre kattintottunk
                /// akkor megkeressük a szelektálós listában és vector elem negálás
                vector<string>::iterator it = find(OsszResz.megfeleloElemekSzelekt.begin(),OsszResz.megfeleloElemekSzelekt.end(),temp);
                if (it != OsszResz.megfeleloElemekSzelekt.end()) { /// ha benne van
                    OsszResz.megfeleloElemekSzelekt.erase(it); /// kivesszük
                } else { /// ha meg nincs benne
                    OsszResz.megfeleloElemekSzelekt.push_back(temp); /// beteszzük
                }
            }
        }
        else if (CsopMent.inClick(MX,MY)){ /// részvények listájának kimentése gomb
            /// a folyamat hibakezelése ki van szervezve
            CsopMentTxt.str=csoportFrissites(OsszResz.megfeleloElemekSzelekt,aktCsopStr);
            if (CsopMentTxt.str=="Siker!") { /// hi minden rendben ment
                aktCsopStr=AktCsop.str; /// akkor eltárolja az aktuális részvényt külön (?)
                aktualisCsoportListaja=csoportReszvenyei(aktCsopStr); /// és befrissíti
                OsszResz.megfeleloElemek=aktualisCsoportListaja;/// a jelenlegi részvényt a fő konténerekbe
            }
        }
        else { /// ha kikattintunk a semmibe
            state=0; /// akkor térjünk vissza a kezdő állapotba
        }

    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de időbélyege nem
        if (state==1){ /// új részvény
            if (isalpha(ev->text.text[0])) /// csak karakterek lehetnek
                ReszMegB.str+=ev->text.text[0];
        }
        if (state==2){ /// új csoport
            if (isalpha(ev->text.text[0])) /// csak karakterek lehetnek
                CsoportMegB.str+=ev->text.text[0];
        }
        if (state==3){ /// részvény törlése
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                ReszMinB.str+=ev->text.text[0];
                ReszMinG.elemekFrissitese(meglevoReszvenyek,ReszMinB.str);
            }
        }
        if (state==4){ /// csoport törlése
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                CsoportMinB.str+=ev->text.text[0];
                CsoportMinG.elemekFrissitese(meglevoCsoportok,CsoportMinB.str);
            }
        }
        if (state==5){ /// csoport kiválasztása
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                AktCsop.str+=ev->text.text[0];
                OsszCsop.elemekFrissitese(meglevoCsoportok,AktCsop.str);
            }
        }
    }

    if (keyDown){ /// ha billentyűzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// előző beviteli mezők tartalmának redukálása és szűrések frissítése
            if (state==1){if (ReszMegB.str.size()>0) ReszMegB.str.pop_back();}
            if (state==2){if (CsoportMegB.str.size()>0) CsoportMegB.str.pop_back();}
            if (state==3){
                if (ReszMinB.str.size()>0) {
                    ReszMinB.str.pop_back();
                    ReszMinG.elemekFrissitese(meglevoReszvenyek,ReszMinB.str);
                }
            }
            if (state==4){
                if (CsoportMinB.str.size()>0) {
                    CsoportMinB.str.pop_back();
                    CsoportMinG.elemekFrissitese(meglevoCsoportok,CsoportMinB.str);
                }
            }
            if (state==5){
                if (AktCsop.str.size()>0) {
                    AktCsop.str.pop_back();
                    OsszCsop.elemekFrissitese(meglevoCsoportok,AktCsop.str);
                }
            }
        }
        if (ev->key.keysym.sym==SDLK_UP){ /// pl. görgetés gyorsítása
            if (state==3) {ReszMinG.speedUpRoll();}
            else if (state==4) {CsoportMinG.speedUpRoll();}
            else if (state==5) {OsszCsop.speedUpRoll();}
            else OsszResz.speedUpRoll();
        }
        if (ev->key.keysym.sym==SDLK_DOWN){ /// lassítása
            if (state==3) {ReszMinG.speedDownRoll();}
            else if (state==4) {CsoportMinG.speedDownRoll();}
            else if (state==5) {OsszCsop.speedDownRoll();}
            else OsszResz.speedDownRoll();
        }
    }

    if (mouseWheel){ /// vagy épp görgetnénk?
        /// görgetőknek átadjuk az irányt, többi az ő bajuk
        if (state==3)ReszMinG.rollIt(-ev->wheel.y);
        else if (state==4)CsoportMinG.rollIt(-ev->wheel.y);
        else if (state==5)OsszCsop.rollIt(-ev->wheel.y);
        else OsszResz.rollIt(-ev->wheel.y);
    }

}

void CsoportEditorMenu::process(){
    /// adatok befrissítése (lehet túl sokszor is)
    int meglevoReszvenyekSize = meglevoReszvenyek.size();
    meglevoReszvenyek = osszesReszveny();
    meglevoCsoportok = osszesCsoport();
    if (meglevoReszvenyek.size()!=(size_t)meglevoReszvenyekSize) /// ha új részvény van
        OsszResz.elemekFrissitese(meglevoReszvenyek); /// a teljes listát befrissíti
    if (firstInitOszzLista){ /// először is egyszer be kell
        firstInitOszzLista=false; /// csak egyszer
        OsszResz.elemekFrissitese(meglevoReszvenyek); /// és frissíteni
    }

    if (oldState!=state){ /// amikor állapotváltozás van
        oldState=state;
        if (state!=1){ /// resetelem a mezők nagyját
            ReszMegB.str="uj reszveny";
        }
        if (state!=2){
            CsoportMegB.str="uj csoport neve";
        }
        if (state!=3){
            ReszMinB.str="reszveny neve V";
            ReszMinG.roll=0; /// és a görgetők tekertségét
        }
        if (state!=4){
            CsoportMinB.str="csoport neve  V";
            CsoportMinG.roll=0;
        }
        if (state!=5){
            AktCsop.str=aktCsopStr;
            OsszCsop.roll=0;
            OsszCsop.elemekFrissitese(meglevoCsoportok,"-1"); /// és szűrök a helytelenre
        }
    }

    if (!kiszamoltState){ /// régi verzió, hogy állapotváltozás közben számolok csak újra
        /// sok adatnál lehet, hogy kelleni fog...
        if (state==0){

        }
    }
}

