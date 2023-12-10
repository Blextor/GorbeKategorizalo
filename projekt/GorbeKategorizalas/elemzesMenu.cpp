#include "Menu.h"

void ElemzesMenu::gombokKialakitasa(){

    FoCim = Text("Elemzes", 20, 10);

    fomenubeB = Button("Fomenu",88,5,52,13,false,false);
    elemzeshezB = Button("ElemzesStart",690,5,100,13,false,false);

    reszInp = Button("",20,120,120,13,false, false);
    reszInpT = Text("Reszveny neve:",20,110);
    reszvenyLista = Gorgetheto({},20,134,120,120);

    reszCsopInp = Button("",20,280,120,13,false, false);
    reszCsopInpT = Text("Reszveny csoport:",20,270);
    reszvenyCsoportLista = Gorgetheto({},20,294,120,120);

    tolT = Text("Datumtol",20,40); evTol = Button("",20,50,44,13,false,false);
    honapTol = Button("",73,50,29,13,false,false);  napTol = Button("",111,50,29,13,false,false);
    igT = Text("Datumig",20,70); evIg = Button("",20,80,44,13,false,false);
    honapIg = Button("",73,80,29,13,false,false); napIg = Button("",111,80,29,13,false,false);

    ujNap = Button("uj Nap",170,5,64,13,false,false); ujNapKomp = Button("uj Nap Komp",249,5,104,13,false,false);
    ujNegyed = Button("uj Negyed",368,5,88,13,false,false); ujNegyedKomp = Button("uj Negyed Komp",471,5,128,13,false,false);
    ujNap.selectable=true; ujNapKomp.selectable=true; ujNegyed.selectable=true; ujNegyedKomp.selectable=true;
    ujNap.selected=true;

    ujFeltetel = Button("Uj Feltetel",170,45,95,13,false,false);
    feltetelT=Text("Feltetelek",175,35);
    ujElemezendo = Button("Uj Elemezendo",488,45,110,13,false,false);
    elemezendoT=Text("Elemezendoek",493,35);
}

void ElemzesMenu::feltetelAdd(bool elemezendo){
    Feltetel f(0,0,303,100);
    f.komper=(tipusState%2==1);
    f.napi=(tipusState<2);
    if (elemezendo){f.x=elemezendoPosX; elemezendoek.push_back(f);}
    else {f.x=feltetelPosX; feltetelek.push_back(f);};
}

void ElemzesMenu::nextMenus(Menu *fomenu, Menu *elemzeshez){
    foMenu=fomenu;
    elemzesFolyamatMenu = elemzeshez;
    feltetelek.clear();
    elemezendoek.clear();
    gombokKialakitasa();
}

void ElemzesMenu::draw() {
    int x, y; /// ablak méretei
    SDL_GetWindowSize(window,&x,&y); /// méretek lekérdezése
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    for (int i=0; i<feltetelek.size(); i++) {
        feltetelek[i].draw(renderer,170,70+i*115);
    }
    for (int i=0; i<elemezendoek.size(); i++) {
        elemezendoek[i].draw(renderer,487,70+i*115);
    }

    reszvenyCsoportLista.draw(renderer,x,y); reszvenyLista.draw(renderer,x,y);

    FoCim.draw(renderer,x,y); fomenubeB.draw(renderer,x,y); elemzeshezB.draw(renderer,x,y);
    reszInp.draw(renderer,x,y); reszInpT.draw(renderer,x,y);
    reszCsopInp.draw(renderer,x,y); reszCsopInpT.draw(renderer,x,y);

    tolT.draw(renderer,x,y); evTol.draw(renderer,x,y);
    honapTol.draw(renderer,x,y); napTol.draw(renderer,x,y);
    igT.draw(renderer,x,y); evIg.draw(renderer,x,y);
    honapIg.draw(renderer,x,y); napIg.draw(renderer,x,y);

    lineRGBA(renderer,165,10,165,410,0,0,0,255);

    ujNap.draw(renderer,x,y); ujNapKomp.draw(renderer,x,y);
    ujNegyed.draw(renderer,x,y); ujNegyedKomp.draw(renderer,x,y);

    feltetelT.draw(renderer,x,y); ujFeltetel.draw(renderer,x,y);
    elemezendoT.draw(renderer,x,y); ujElemezendo.draw(renderer,x,y);

    lineRGBA(renderer,482,30,482,410,0,0,0,255);

    SDL_RenderPresent(renderer);
}


void ElemzesMenu::inputHandle(){
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
            /// kérdés, hogy eltaláltunk-e valami kattinthatót
            if (fomenubeB.inClick(MX,MY)) *menu = foMenu;
            else if (elemzeshezB.inClick(MX,MY)) *menu = elemzesFolyamatMenu;

            if (evTol.inClick(MX,MY)) datumChState=1;
            else if (honapTol.inClick(MX,MY)) datumChState=2;
            else if (napTol.inClick(MX,MY)) datumChState=3;
            else if (evIg.inClick(MX,MY)) datumChState=4;
            else if (honapIg.inClick(MX,MY)) datumChState=5;
            else if (napIg.inClick(MX,MY)) datumChState=6;
            else datumChState=0;

            if (reszInp.inClick(MX,MY)) reszState=1;
            else if (reszCsopInp.inClick(MX,MY)) reszState=2;
            else if (reszvenyLista.inClick(MX,MY)) {
                reszState=1;
                /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
                string btStr = reszvenyLista.whichButton(MX,MY);
                if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                    reszInp.str=btStr;
                    reszvenyLista.elemekKeresese(btStr);
                } else {/// csúszka használatra fenntartva

                }
            }
            else if (reszvenyCsoportLista.inClick(MX,MY)) {
                reszState=2;
                /// görgető-be kattintva kérdéses még, hogy hova is érkezett a kattintás
                string btStr = reszvenyCsoportLista.whichButton(MX,MY);
                if (btStr!=""){ /// ha siker, akkor szövegét használjuk
                    reszCsopInp.str=btStr;
                    reszvenyCsoportLista.elemekKeresese(btStr);
                } else {/// csúszka használatra fenntartva

                }
            }
            else reszState=0;
            if (reszState==1) {
                reszvenyLista.elemekFrissitese(osszesReszveny());
                reszvenyCsoportLista.elemekKeresese("???");
            }
            else if (reszState==2) {
                reszvenyCsoportLista.elemekFrissitese(osszesCsoport());
                reszvenyLista.elemekKeresese("???");
            }

            if (ujNap.inClick(MX,MY)) tipusState=0;
            else if (ujNapKomp.inClick(MX,MY)) tipusState=1;
            else if (ujNegyed.inClick(MX,MY)) tipusState=2;
            else if (ujNegyedKomp.inClick(MX,MY)) tipusState=3;
            ujNap.selected=(tipusState==0);
            ujNapKomp.selected=(tipusState==1);
            ujNegyed.selected=(tipusState==2);
            ujNegyedKomp.selected=(tipusState==3);

            for (int i=0; i<feltetelek.size() && MY>=70; i++){
                if (feltetelek[i].inClick(MX,MY)){
                    feltetelek.erase(feltetelek.begin()+i);
                    i--;
                }
            }
            for (int i=0; i<elemezendoek.size() && MY>=70; i++){
                if (elemezendoek[i].inClick(MX,MY)){
                    elemezendoek.erase(elemezendoek.begin()+i);
                    i--;
                }
            }

            if (ujFeltetel.inClick(MX,MY)) feltetelAdd(false);
            if (ujElemezendo.inClick(MX,MY)) feltetelAdd(true);
        }
    }

    int tx, ty;
    Uint32 mouseState = SDL_GetMouseState(&tx, &ty);
    if (ev->type==SDL_MOUSEMOTION && 0<(mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))){
        ///panX+=MoveX;
        ///panY+=MoveY;
        //if (panX<0) panX=0;
        //if (panY<0) panY=0;
    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// szövegbevitel esemény végtelen, de idõbélyege nem
        if (datumChState==1) evTol.str+=ev->text.text[0];
        else if (datumChState==2) honapTol.str+=ev->text.text[0];
        else if (datumChState==3) napTol.str+=ev->text.text[0];
        else if (datumChState==4) evIg.str+=ev->text.text[0];
        else if (datumChState==5) honapIg.str+=ev->text.text[0];
        else if (datumChState==6) napIg.str+=ev->text.text[0];

        if (reszState==1) reszInp.str+=ev->text.text[0];
        else if (reszState==2) reszCsopInp.str+=ev->text.text[0];

        for (int i=0; i<feltetelek.size(); i++){
            feltetelek[i].inputText(ev->text.text[0]);
        }
        for (int i=0; i<elemezendoek.size(); i++){
            elemezendoek[i].inputText(ev->text.text[0]);
        }

    }

    if (keyDown){ /// ha billentyûzetet nem gépelés miatt ütöttük le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. törlés miatt
            /// elõzõ beviteli mezõk tartalmának redukálása és szûrések frissítése
            if (datumChState==1) {if (evTol.str.size()>0) evTol.str.pop_back();}
            else if (datumChState==2) {if (honapTol.str.size()>0)  honapTol.str.pop_back();}
            else if (datumChState==3) {if (napTol.str.size()>0)  napTol.str.pop_back();}
            else if (datumChState==4) {if (evIg.str.size()>0)  evIg.str.pop_back();}
            else if (datumChState==5) {if (honapIg.str.size()>0)  honapIg.str.pop_back();}
            else if (datumChState==6) {if (napIg.str.size()>0)  napIg.str.pop_back();}

            if (reszState==1) {if (reszInp.str.size()>0) reszInp.str.pop_back();}
            else if (reszState==2) {if (reszCsopInp.str.size()>0) reszCsopInp.str.pop_back();}

            for (int i=0; i<feltetelek.size(); i++){
                feltetelek[i].inputText(' ',true);
            }
            for (int i=0; i<elemezendoek.size(); i++){
                elemezendoek[i].inputText(' ',true);
            }
        }

        if (ev->key.keysym.sym==SDLK_UP){ /// pl. görgetés gyorsítása
            if (reszState==1) {reszvenyLista.speedUpRoll();}
            if (reszState==2) {reszvenyCsoportLista.speedUpRoll();}
        }
        if (ev->key.keysym.sym==SDLK_DOWN){ /// lassítása
            if (reszState==1) {reszvenyLista.speedDownRoll();}
            if (reszState==2) {reszvenyCsoportLista.speedDownRoll();}
        }
        if (ev->key.keysym.sym==SDLK_LEFT){ /// feltétel lista reset
            panYFelt=0;
        }
        if (ev->key.keysym.sym==SDLK_RIGHT){ /// elemezendő lista reset
            panYElemez=0;
        }
    }

    if (mouseWheel){ /// vagy épp görgetnénk?
        /// görgetőknek átadjuk az irányt, többi az ő bajuk
        if (reszState==1) reszvenyLista.rollIt(-ev->wheel.y);
        else if (reszState==2)reszvenyCsoportLista.rollIt(-ev->wheel.y);
    }

    if (state!=oldState){
        if (oldState==1){
            //ujReszInp.str="reszveny neve";
            //reszvenyLista.elemekKeresese();
        }
    }
}

void ElemzesMenu::process(){
    //SDL_SetWindowSize(window,800,600);
    meglevoReszvenyek = osszesReszveny();
    meglevoCsoportok = osszesCsoport();
    if (reszState==1){reszvenyLista.elemekKeresese(reszInp.str,true); reszCsopInp.str="";}
    if (reszState==2){reszvenyCsoportLista.elemekKeresese(reszCsopInp.str,true); reszInp.str="";}
}
