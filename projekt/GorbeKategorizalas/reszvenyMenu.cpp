#include "Menu.h"

void ReszvenyMenu::gombokKialakitasa(){
    FoCim = Text("Reszvenyek megtekintese", 20, 20);

    FoMB = Button("Fomenu",250,15,52,13,false,false);
    ElmMB = Button("Uj elemzes",320,15,84,13,false,false);
    CimMB = Button("Uj cimke",422,15,68,13,false,false);

    ujReszInp = Button("reszveny neve",20,50,120,13,false, false);
    reszvenyLista = Gorgetheto(osszesReszveny(),20,70,124,102,false,false);
    reszOKB = Button("OK",150,50,32,13,false, false);

    ujCimkePopUp = UjCimkePopUp(0,0,500,400);
}


void ReszvenyMenu::nextMenus(Menu *fomenu, Menu *elemzes){
    foMenu=fomenu;
    elemzoMenu=elemzes;
    boolR.resize(10);
    reszvenyek.resize(10);
    gombokKialakitasa();
}

void ReszvenyMenu::draw() {
    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);



    for (size_t i=0; i<reszvenyek.size(); i++)
        if (boolR[i])
            reszvenyek[i].draw(renderer,30+panX,100+220*i+panY);

    /// elfedő réteg
    boxRGBA(renderer,0,0,500,80,100,100,100,255);
    rectangleRGBA(renderer,0,0,500,80,0,0,0,255);

    FoCim.draw(renderer,x,y);
    FoMB.draw(renderer,x,y);
    ElmMB.draw(renderer,x,y);
    CimMB.draw(renderer,x,y);

    if (state == 1)
        reszvenyLista.draw(renderer,x,y);
    ujReszInp.draw(renderer,x,y);
    reszOKB.draw(renderer,x,y);

    if (ujCimkePopUpB)
        ujCimkePopUp.draw(renderer,x,y);

    SDL_RenderPresent(renderer);
}

bool ReszvenyMenu::reszvenyekClick(int bx, int by){
    for (size_t i=0; i<boolR.size(); i++){
        if (boolR[i]){
            if (reszvenyek[i].inClick(bx,by))
                return true;
        }
    }
    return false;
}

void ReszvenyMenu::inputHandle(){
    int oldState=state;
    int MX=-1, MY=-1; /// kurzor pozíciója, ha -1 marad, nem történt változás
    int MoveX=0, MoveY=0; /// kurzor pozíciója, ha -1 marad, nem történt változás
    bool leftButton = false; /// külön kígyûjtöm, hogy lenyomták-e a bal egérgombot
    bool keyDown = false; /// vagy bármelyt a billentyûzeten
    bool mouseWheel = false; /// vagy görgettek-e

    if (SDL_PollEvent(ev)){ /// lekérem az eseményt és kigyûjtöm, hogy mi történt
        if (ev->type==SDL_MOUSEBUTTONDOWN){ /// csak kattintáskor kérem le az egér pozíciót
            MX=ev->button.x;
            MY=ev->button.y;
            leftButton=ev->button.button==SDL_BUTTON_LEFT; /// bal gomb
            //middleButton=ev->button.button==SDL_BUTTON_MIDDLE; /// bal gomb
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            if (ev->motion.state!=-1){
                MoveX = ev->motion.xrel;
                MoveY = ev->motion.yrel;
            }
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
        if (leftButton){
            /// Új címke popUp

            if (ujCimkePopUpB){ /// Új címke popUp aktív eset
                ujCimkePopUp.inClick(MX,MY);
                if (ujCimkePopUp.closeB.inClick(MX,MY)) ujCimkePopUpB=false;
                else if (ujCimkePopUp.cimkeTipusLista.inClick(MX,MY)){ /// cípketípus kiválasztása
                    /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
                    string btStr = ujCimkePopUp.cimkeTipusLista.whichButton(MX,MY);
                    if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                        ujCimkePopUp.typeInput.str=btStr;
                        ujCimkePopUp.cimkeTipusLista.elemekKeresese(ujReszInp.str);
                        ujCimkePopUp.generateInputs();
                    } else {/// csúszka használatra fenntartva

                    }
                }
                else if (ujCimkePopUp.ujLetrehozas.inClick(MX,MY)){
                    if (ujCimkePopUp.ujCimkeLetrehozasa()){
                        ujCimkePopUp.lastInputField=0;
                        ujCimkePopUp.lastType=-1;
                        ujCimkePopUpB=false;
                    }
                }
                //else continue;
                //break;
            }
            else if (CimMB.inClick(MX,MY)) ujCimkePopUpB=true;


            /// kérdés, hogy eltaláltunk-e valami kattinthatót
            else if (FoMB.inClick(MX,MY)) *menu = foMenu;
            else if (ElmMB.inClick(MX,MY)) *menu = elemzoMenu;
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
                for (size_t i=0; i<boolR.size(); i++){
                    if (!boolR[i]){
                        boolR[i]=true;
                        if (!reszvenyek[i].setStock(ujReszInp.str))
                            boolR[i]=false;
                        break;
                    }
                }
                state=0;
            }
            else if (reszvenyekClick(MX,MY)){

            }
            else { /// ha kikattintunk a semmibe
                state=0; /// akkor térjünk vissza a kezdõ állapotba
            }
        }
    }

    int tx, ty;
    Uint32 mouseState = SDL_GetMouseState(&tx, &ty);
    if (ev->type==SDL_MOUSEMOTION && 0<(mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))){
        panX+=MoveX;
        panY+=MoveY;
        //if (panX<0) panX=0;
        //if (panY<0) panY=0;
    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de idõbélyege nem

        if (ujCimkePopUpB){
            ujCimkePopUp.inputText(ev->text.text[0]);
        }
        else if (state==1){ /// új részvény
            if (isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                ujReszInp.str+=ev->text.text[0];
                reszvenyLista.elemekKeresese(ujReszInp.str);
            }
        }
        else {
            for (size_t i=0; i<reszvenyek.size(); i++){
                reszvenyek[i].inInput(ev->text.text[0]);
            }
        }

    }

    if (keyDown){ /// ha billentyûzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// elõzõ beviteli mezõk tartalmának redukálása és szûrések frissítése

            if (ujCimkePopUpB){
                ujCimkePopUp.inputText(' ',true);
            }
            else if (state==1){
                if (ujReszInp.str.size()>0) {
                    ujReszInp.str.pop_back();
                    reszvenyLista.elemekKeresese(ujReszInp.str);
                }
            } else {
                for (size_t i=0; i<reszvenyek.size(); i++){
                    reszvenyek[i].inDeleteButton();
                }
            }

        }

        if (ev->key.keysym.sym==SDLK_UP){ /// pl. görgetés gyorsítása
            if (state==1) {reszvenyLista.speedUpRoll();}
        }
        if (ev->key.keysym.sym==SDLK_DOWN){ /// lassítása
            if (state==1) {reszvenyLista.speedDownRoll();}
        }
        if (ev->key.keysym.sym==SDLK_LEFT){ /// lassítása
            panX=0; panY=0;
        }
    }

    if (mouseWheel){ /// vagy épp görgetnénk?
        /// görgetőknek átadjuk az irányt, többi az ő bajuk
        if (ujCimkePopUpB) ujCimkePopUp.cimkeTipusLista.rollIt(-ev->wheel.y);
        else if (state==1)reszvenyLista.rollIt(-ev->wheel.y);
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
