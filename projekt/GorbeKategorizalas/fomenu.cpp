#include "Menu.h"

void FoMenu::gombokKialakitasa(){
    /// fejléc szövegek
    FoCim = Text("Gorbe kategorizalo",100,100);

    /// menü váltó gombok
    FrissMB = Button("Adatok frissitese",100,200,150,13,false,false);
    RCSMB = Button("Csoportositas",300,200,112,13,false,false);
    GorbeMB = Button("Reszvenyek",100,150,82,13,false,false);
    ElemzesMB = Button("Elemzes",300,150,58,13,false,false);
    KilepB = Button("Kilepes",-80,-50,62,13,true,true);


    szalakKiv = Text("Hany szalon fusson maximum:",50,400);

    stringstream ss; ss<<Config.getMaxThreads();
    string str = ss.str();
    szalMennyisegB = Button(str,270,395,32,13,false,false);

    szalakKivInfo = Text("(Szalankent szukseges lehet 512 Mb RAM)",50,430);

}


void FoMenu::nextMenus(Menu *frissito, Menu *csoport, Menu *gorbe){
    frissitoMenu=frissito;
    csoportEditormenu=csoport;
    gorbeMenu=gorbe;
    gombokKialakitasa();
}

void FoMenu::draw() {
    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    FoCim.draw(renderer,x,y);
    szalakKiv.draw(renderer,x,y);
    FrissMB.draw(renderer,x,y);
    RCSMB.draw(renderer,x,y);
    GorbeMB.draw(renderer,x,y);
    ElemzesMB.draw(renderer,x,y);
    KilepB.draw(renderer,x,y);

    szalMennyisegB.draw(renderer,x,y);
    szalakKivInfo.draw(renderer,x,y);

    SDL_RenderPresent(renderer);
}

void FoMenu::inputHandle(){
    int oldState=state;
    int MX=-1, MY=-1; /// kurzor pozíciója, ha -1 marad, nem történt változás
    //bool leftButton = true; /// külön kígyûjtöm, hogy lenyomták-e a bal egérgombot
    bool keyDown = false; /// vagy bármelyt a billentyûzeten
    //bool mouseWheel = false; /// vagy görgettek-e

    if (SDL_PollEvent(ev)){ /// lekérem az eseményt és kigyûjtöm, hogy mi történt
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
            ///mouseWheel=true;
        }
        if (ev->type==SDL_KEYDOWN){ /// billentyû
            keyDown=true;
            //if (ev->key.keysym.sym==SDLK_r)
        }
        if (ev->type == SDL_QUIT) /// beégetett kilépés engedélyezése
            exit(3);
    }

    if (MX!=-1){ /// azaz az egérrel kattintottunk
        /// kérdés, hogy eltaláltunk-e valami kattinthatót
        if (FrissMB.inClick(MX,MY)) *menu = frissitoMenu; /// fõmenübe írányító gombot
        else if (RCSMB.inClick(MX,MY)) *menu = csoportEditormenu; /// csoport szerksztõ menü gombot
        else if (GorbeMB.inClick(MX,MY)) *menu = gorbeMenu; /// részvény menü gombot
        else if (KilepB.inClick(MX,MY)) exit(3); /// csoport szerksztõ menü gombot
        else if (szalMennyisegB.inClick(MX,MY)) {
            state=1;
        }

        else { /// ha kikattintunk a semmibe
            state=0; /// akkor térjünk vissza a kezdõ állapotba
        }

    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de idõbélyege nem
        if (state==1){ /// új részvény
            if (isalnum(ev->text.text[0]) && !isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                szalMennyisegB.str+=ev->text.text[0];
            }
        }

    }

    if (keyDown){ /// ha billentyûzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// elõzõ beviteli mezõk tartalmának redukálása és szûrések frissítése
            if (state==1){
                if (szalMennyisegB.str.size()>0) {
                    szalMennyisegB.str.pop_back();
                }
            }
        }
    }

    if (state!=oldState){
        if (oldState==1){
            stringstream ss(szalMennyisegB.str);
            int val; ss>>val;
            Config.setMaxThreads(val);
            ss.str(" "); ss.clear(); ss<<Config.getMaxThreads();
            //cout<<ss.str()<<endl;
            szalMennyisegB.str=ss.str();
            //cout<<"valma "<<Config.getMaxThreads()<<endl;
        }
    }
}

void FoMenu::process(){

}


