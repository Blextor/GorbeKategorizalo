#include "Menu.h"

void ReszvenyMenu::gombokKialakitasa(){
    FoCim = Text("Reszvenyek megtekintese", 20, 20);

    FoMB = Button("Fomenu",250,15,52,13,false,false);
    ElmMB = Button("Uj elemzes",320,15,84,13,false,false);
    CimMB = Button("Uj cimke",422,15,68,13,false,false);

    ujReszInp = Button("reszveny neve",20,50,120,13,false, false);
    reszvenyLista = Gorgetheto(osszesReszveny(),20,70,124,102,false,false);
    reszOKB = Button("OK",150,50,32,13,false, false);
}


void ReszvenyMenu::nextMenus(Menu *fomenu){
    foMenu=fomenu;
    boolR.resize(10);
    reszvenyek.resize(10);
    gombokKialakitasa();
}

void ReszvenyMenu::draw() {
    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    FoCim.draw(renderer,x,y);
    FoMB.draw(renderer,x,y);
    ElmMB.draw(renderer,x,y);
    CimMB.draw(renderer,x,y);


    for (int i=0; i<reszvenyek.size(); i++)
        if (boolR[i])
            reszvenyek[i].draw(renderer,50,100+220*i);


    if (state == 1)
        reszvenyLista.draw(renderer,x,y);
    ujReszInp.draw(renderer,x,y);
    reszOKB.draw(renderer,x,y);

    SDL_RenderPresent(renderer);
}

void ReszvenyMenu::inputHandle(){
    int oldState=state;
    int MX=-1, MY=-1; /// kurzor pozíciója, ha -1 marad, nem történt változás
    //bool leftButton = true; /// külön kígyûjtöm, hogy lenyomták-e a bal egérgombot
    bool keyDown = false; /// vagy bármelyt a billentyûzeten
    bool mouseWheel = false; /// vagy görgettek-e

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
            mouseWheel=true;
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
        if (FoMB.inClick(MX,MY)) *menu = foMenu;
        else if (ujReszInp.inClick(MX,MY)){
            state = 1;
            ujReszInp.str="";
            reszvenyLista.elemekKeresese(ujReszInp.str);
        }
        else if (state == 1 && reszvenyLista.inClick(MX,MY)){
            /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
            string btStr = reszvenyLista.whichButton(MX,MY);
            if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                ujReszInp.str=btStr;
                reszvenyLista.elemekKeresese(ujReszInp.str);
            } else {/// csúszka használatra fenntartva

            }
        }
        else if (reszOKB.inClick(MX,MY)) {
            for (int i=0; i<boolR.size(); i++){
                if (!boolR[i]){
                    boolR[i]=true;
                    if (!reszvenyek[i].setStock(ujReszInp.str))
                        boolR[i]=false;
                    break;
                }
            }
            state=0;
        }
        else { /// ha kikattintunk a semmibe
            state=0; /// akkor térjünk vissza a kezdõ állapotba
        }
        /*
        if (FrissMB.inClick(MX,MY)) *menu = frissitoMenu; /// fõmenübe írányító gombot
        else if (RCSMB.inClick(MX,MY)) *menu = csoportEditormenu; /// csoport szerksztõ menü gombot
        else if (KilepB.inClick(MX,MY)) exit(3); /// csoport szerksztõ menü gombot
        else if (szalMennyisegB.inClick(MX,MY)) {
            state=1;
        }

        else { /// ha kikattintunk a semmibe
            state=0; /// akkor térjünk vissza a kezdõ állapotba
        }
        */
    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de idõbélyege nem

        if (state==1){ /// új részvény
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                ujReszInp.str+=ev->text.text[0];
                reszvenyLista.elemekKeresese(ujReszInp.str);
            }
        }

    }

    if (keyDown){ /// ha billentyûzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// elõzõ beviteli mezõk tartalmának redukálása és szûrések frissítése

            if (state==1){
                if (ujReszInp.str.size()>0) {
                    ujReszInp.str.pop_back();
                    reszvenyLista.elemekKeresese(ujReszInp.str);
                }
            }

        }

        if (ev->key.keysym.sym==SDLK_UP){ /// pl. görgetés gyorsítása
            if (state==1) {reszvenyLista.speedUpRoll();}
        }
        if (ev->key.keysym.sym==SDLK_DOWN){ /// lassítása
            if (state==1) {reszvenyLista.speedDownRoll();}
        }
    }

    if (mouseWheel){ /// vagy épp görgetnénk?
        /// görgetőknek átadjuk az irányt, többi az ő bajuk
        if (state==1)reszvenyLista.rollIt(-ev->wheel.y);
    }

    if (state!=oldState){
        if (oldState==1){
            ujReszInp.str="reszveny neve";
            reszvenyLista.elemekKeresese();
        }
    }
}

void ReszvenyMenu::process(){

}
