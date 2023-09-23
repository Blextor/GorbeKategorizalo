#include "Menu.h"

void FrissitoMenu::gombokKialakitasa(){
    /// fejléc szövegek
    FoCim = Text("Adatok letoltese es frissitese",15,15);
    kisLeiras = Text("Letoltes nullarol kezd, a frissites felhasznalja ami van.",15,30);

    /// menü váltó gombok
    FMB = Button("FoMenu",15,35,52,13,false,true);
    RCSMB = Button("Csoportosito",90,35,100,13,false,true);

    /// Részvény letöltése
    ReszBevB = Button("reszveny neve V",20,70,124,13,false,false);
    ReszBevG = Gorgetheto(meglevoReszvenyek,20,85,124,102,false,false);
    ReszLetB = Button("letoltes",160,70,76,13,false,false);
    ReszFrissB = Button("frissites",160,100,76,13,false,false);
    ReszTxt = Text("reszveny",162,55);
    ReszVissz = Text("",162,129);

    /// Részvény letöltése
    CsopBevB = Button("csoport neve  V",340,70,124,13,false,false);
    CsopBevG = Gorgetheto(meglevoReszvenyek,340,85,124,102,false,false);
    CsopLetB = Button("letoltes",480,70,76,13,false,false);
    CsopFrissB = Button("frissites",480,100,76,13,false,false);
    CsopTxt = Text("csoport",482,55);
    CsopVissz = Text("",482,129);

    /// ProgressBar
    kivalasztottDologT = Text("",32,280);
    progBar = ProgressBar(30,300,580,25,false,false);
    PSSB = Button("start",495,275,43,13,false,false);
    PCB = Button("megsem",555,275,50,13,false,false);

}

void FrissitoMenu::nextMenus(Menu *fo, Menu *csoport){
    fomenu=fo;
    csoportEditormenu=csoport;
    gombokKialakitasa();
}

void FrissitoMenu::draw() {

    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    /// fejlécek
    FoCim.draw(renderer,x,y);
    kisLeiras.draw(renderer,x,y);

    /// menü gombok
    FMB.draw(renderer,x,y);
    RCSMB.draw(renderer,x,y);

    /// részvény letöltése
    ReszBevG.draw(renderer,x,y);
    ReszBevB.draw(renderer,x,y);
    ReszLetB.draw(renderer,x,y);
    ReszFrissB.draw(renderer,x,y);
    ReszTxt.draw(renderer,x,y);
    ReszVissz.draw(renderer,x,y);

    /// csoport letöltése
    CsopBevG.draw(renderer,x,y);
    CsopBevB.draw(renderer,x,y);
    CsopLetB.draw(renderer,x,y);
    CsopFrissB.draw(renderer,x,y);
    CsopTxt.draw(renderer,x,y);
    CsopVissz.draw(renderer,x,y);

    /// szeparátor
    lineRGBA(renderer,320,55,320,200,0,0,0,255);

    /// progressBar
    kivalasztottDologT.draw(renderer,x,y);
    progBar.draw(renderer,x,y);
    PSSB.draw(renderer,x,y);
    PCB.draw(renderer,x,y);

    SDL_RenderPresent(renderer);
}

void FrissitoMenu::inputHandle() {
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
        else if (RCSMB.inClick(MX,MY)) *menu = csoportEditormenu; /// csoport szerksztő menü gombot
        else if (ReszBevB.inClick(MX,MY)){
            ReszVissz.str="";
            if (state!=1){ /// ha nem nyomtunk még bele
                ReszBevB.str="";    /// törli a szöveget
                ReszBevG.elemekFrissitese(meglevoReszvenyek); /// és lekérdezi a részvényeket
            }
            state=1;
        }
        else if (CsopBevB.inClick(MX,MY)){
            CsopVissz.str="";
            if (state!=2){ /// ha nem nyomtunk még bele
                CsopBevB.str="";    /// törli a szöveget
                CsopBevG.elemekFrissitese(meglevoCsoportok); /// és lekérdezi a részvényeket
            }
            state=2;
        }
        else if (ReszLetB.inClick(MX,MY)){
            if (state==1){
                if (ReszBevB.str.size()==0) ReszVissz.str = "Ures!";
                else ReszVissz.str = reszvenyLetoltesChk(ReszBevB.str);
                if (ReszVissz.str=="Siker!"){
                    letoltesValasztva=true;
                    kivReszSet(ReszBevB.str);
                    kivalasztottDologT.str="letolteni kivant reszveny: "+kivResz;
                }
            }
            state=0;
        }
        else if (CsopLetB.inClick(MX,MY)){
            if (state==2){
                if (CsopBevB.str.size()==0) CsopVissz.str = "Ures!";
                else CsopVissz.str = csoportLetoltesChk(CsopBevB.str);
                if (CsopVissz.str=="Siker!"){
                    letoltesValasztva=true;
                    kivCsopSet(CsopBevB.str);
                    kivalasztottDologT.str="letolteni kivant csoport: "+kivCsop;
                }
            }
            state=0;
        }
        else if (ReszFrissB.inClick(MX,MY)){
            if (state==1){
                if (ReszBevB.str.size()==0) ReszVissz.str = "Ures!";
                else ReszVissz.str = reszvenyFrissitesChk(ReszBevB.str);
                if (ReszVissz.str=="Siker!"){
                    letoltesValasztva=false;
                    kivReszSet(ReszBevB.str);
                    kivalasztottDologT.str="frissiteni kivant csoport: "+kivResz;
                }
            }
            state=0;
        }
        else if (CsopFrissB.inClick(MX,MY)){
            if (state==2){
                if (CsopBevB.str.size()==0) CsopVissz.str = "Ures!";
                else CsopVissz.str = csoportFrissitesChk(CsopBevB.str);
                if (CsopVissz.str=="Siker!"){
                    letoltesValasztva=false;
                    kivCsopSet(CsopBevB.str);
                    kivalasztottDologT.str="frissiteni kivant csoport: "+kivCsop;
                }
            }
            state=0;
        }
        else if (state == 1 && ReszBevG.inClick(MX,MY)){ /// részvény lista
            /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
            string btStr = ReszBevG.whichButton(MX,MY);
            if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                ReszBevB.str=btStr;
                ReszBevG.elemekFrissitese(meglevoReszvenyek,btStr);
            } else {/// csúszka használatra fenntartva

            }
        }
        else if (state == 2 && CsopBevG.inClick(MX,MY)){ /// részvény lista
            /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
            string btStr = CsopBevG.whichButton(MX,MY);
            if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                CsopBevB.str=btStr;
                CsopBevG.elemekFrissitese(meglevoCsoportok,btStr);
            } else {/// csúszka használatra fenntartva

            }
        }
        else if (PSSB.inClick(MX,MY)){
            startStop();
        }
        else if (PCB.inClick(MX,MY)){
            cancel();
        }
        else { /// ha kikattintunk a semmibe
            state=0; /// akkor térjünk vissza a kezdő állapotba
        }

    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de időbélyege nem
        progBar.elemFeldolgozva();
        if (state==1){ /// új részvény
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                ReszBevB.str+=ev->text.text[0];
                ReszBevG.elemekFrissitese(meglevoReszvenyek,ReszBevB.str);
            }
        }
        if (state==2){ /// új részvény
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                CsopBevB.str+=ev->text.text[0];
                CsopBevG.elemekFrissitese(meglevoCsoportok,CsopBevB.str);
            }
        }

    }

    if (keyDown){ /// ha billentyűzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// előző beviteli mezők tartalmának redukálása és szűrések frissítése
            if (state==1){
                if (ReszBevB.str.size()>0) {
                    ReszBevB.str.pop_back();
                    ReszBevG.elemekFrissitese(meglevoReszvenyek,ReszBevB.str);
                }
            }
            if (state==2){
                if (CsopBevB.str.size()>0) {
                    CsopBevB.str.pop_back();
                    CsopBevG.elemekFrissitese(meglevoCsoportok,CsopBevB.str);
                }
            }
            /*
            if (state==3){
                if (ReszMinB.str.size()>0) {
                    ReszMinB.str.pop_back();
                    ReszMinG.elemekFrissitese(meglevoReszvenyek,ReszMinB.str);
                }
            }
            */
        }
        if (ev->key.keysym.sym==SDLK_UP){ /// pl. görgetés gyorsítása
            if (state==1) {ReszBevG.speedUpRoll();}
            if (state==2) {CsopBevG.speedUpRoll();}
        }
        if (ev->key.keysym.sym==SDLK_DOWN){ /// lassítása
            if (state==1) {ReszBevG.speedDownRoll();}
            if (state==2) {CsopBevG.speedDownRoll();}
        }
    }

    if (mouseWheel){ /// vagy épp görgetnénk?
        /// görgetőknek átadjuk az irányt, többi az ő bajuk
        if (state==1)ReszBevG.rollIt(-ev->wheel.y);
        if (state==2)CsopBevG.rollIt(-ev->wheel.y);
    }


}

void FrissitoMenu::process(){
    /// adatok befrissítése (lehet túl sokszor is)
    //int meglevoReszvenyekSize = meglevoReszvenyek.size();
    meglevoReszvenyek = osszesReszveny();
    meglevoCsoportok = osszesCsoport();

    //if (letoltoSzal.joinable()){cout<<"join"<<endl;}

    if (oldState!=state){ /// amikor állapotváltozás van
        oldState=state;
        if (state!=1){ /// resetelem a mezők nagyját
            ReszBevB.str="reszveny neve V";
            ReszBevG.roll=0;
            ReszBevG.elemekFrissitese(meglevoCsoportok,"-1");
        }
        if (state!=2){
            CsopBevB.str="csoport neve  V";
            CsopBevG.roll=0; /// és a görgetők tekertségét
            CsopBevG.elemekFrissitese(meglevoCsoportok,"-1");
        }
        if (state!=5){
            //AktCsop.str=aktCsopStr;
            //OsszCsop.roll=0;
            //OsszCsop.elemekFrissitese(meglevoCsoportok,"-1"); /// és szűrök a helytelenre
        }
    }
}

