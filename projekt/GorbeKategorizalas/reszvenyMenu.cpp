#include "Menu.h"

void ReszvenyMenu::gombokKialakitasa(){
    FoCim = Text("Reszvenyek megtekintese", 20, 20);

    FoMB = Button("Fomenu",250,15,52,13,false,false);
    ElmMB = Button("Uj elemzes",320,15,84,13,false,false);
    CimMB = Button("Uj cimke",422,15,68,13,false,false);

    ujReszInp = Button("reszveny neve",20,50,120,13,false, false);
    reszvenyLista = Gorgetheto(osszesReszveny(),20,70,120,90);
    reszOKB = Button("OK",150,50,32,13,false, false);
}


void ReszvenyMenu::nextMenus(Menu *fomenu){
    foMenu=fomenu;
    boolR.resize(10);
    reszvenyek.resize(10);
    gombokKialakitasa();
}

void ReszvenyMenu::draw() {
    int x, y; /// ablak m�retei
    SDL_GetWindowSize(window,&x,&y); /// m�retek lek�rdez�se
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    FoCim.draw(renderer,x,y);
    FoMB.draw(renderer,x,y);
    ElmMB.draw(renderer,x,y);
    CimMB.draw(renderer,x,y);

    if (state == 1)
        reszvenyLista.draw(renderer,x,y);
    ujReszInp.draw(renderer,x,y);
    reszOKB.draw(renderer,x,y);

    for (int i=0; i<reszvenyek.size(); i++)
        if (boolR[i])
            reszvenyek[i].draw(renderer,50,100+220*i);

    SDL_RenderPresent(renderer);
}

void ReszvenyMenu::inputHandle(){
    int oldState=state;
    int MX=-1, MY=-1; /// kurzor poz�ci�ja, ha -1 marad, nem t�rt�nt v�ltoz�s
    //bool leftButton = true; /// k�l�n k�gy�jt�m, hogy lenyomt�k-e a bal eg�rgombot
    bool keyDown = false; /// vagy b�rmelyt a billenty�zeten
    bool mouseWheel = false; /// vagy g�rgettek-e

    if (SDL_PollEvent(ev)){ /// lek�rem az esem�nyt �s kigy�jt�m, hogy mi t�rt�nt
        if (ev->type==SDL_MOUSEBUTTONDOWN){ /// csak kattint�skor k�rem le az eg�r poz�ci�t
            MX=ev->button.x;
            MY=ev->button.y;
            //leftButton=ev->button.button==SDL_BUTTON_LEFT; /// bal gomb
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        }
        if (ev->type==SDL_MOUSEWHEEL){ /// g�rgt�s
            mouseWheel=true;
        }
        if (ev->type==SDL_KEYDOWN){ /// billenty�
            keyDown=true;
            //if (ev->key.keysym.sym==SDLK_r)
        }
        if (ev->type == SDL_QUIT) /// be�getett kil�p�s enged�lyez�se
            exit(3);
    }

    if (MX!=-1){ /// azaz az eg�rrel kattintottunk
        /// k�rd�s, hogy eltal�ltunk-e valami kattinthat�t
        if (FoMB.inClick(MX,MY)) *menu = foMenu;
        else if (reszOKB.inClick(MX,MY)) {
            for (int i=0; i<boolR.size(); i++){
                if (!boolR[i]){
                    boolR[i]=true;
                    reszvenyek[i].setStock("DBX");
                    break;
                }
            }
        }
        else { /// ha kikattintunk a semmibe
            state=0; /// akkor t�rj�nk vissza a kezd� �llapotba
        }
        /*
        if (FrissMB.inClick(MX,MY)) *menu = frissitoMenu; /// f�men�be �r�ny�t� gombot
        else if (RCSMB.inClick(MX,MY)) *menu = csoportEditormenu; /// csoport szerkszt� men� gombot
        else if (KilepB.inClick(MX,MY)) exit(3); /// csoport szerkszt� men� gombot
        else if (szalMennyisegB.inClick(MX,MY)) {
            state=1;
        }

        else { /// ha kikattintunk a semmibe
            state=0; /// akkor t�rj�nk vissza a kezd� �llapotba
        }
        */
    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// sz�vegbevitel esem�ny v�gtelen, de id�b�lyege nem
        /*
        if (state==1){ /// �j r�szv�ny
            if (isalnum(ev->text.text[0]) && !isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                szalMennyisegB.str+=ev->text.text[0];
            }
        }
        */
    }

    if (keyDown){ /// ha billenty�zetet nem g�pel�s miatt �t�tt�k le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. t�rl�s miatt
            /// el�z� beviteli mez�k tartalm�nak reduk�l�sa �s sz�r�sek friss�t�se
            /*
            if (state==1){
                if (szalMennyisegB.str.size()>0) {
                    szalMennyisegB.str.pop_back();
                }
            }
            */
        }
    }

    if (state!=oldState){
        if (oldState==1){

        }
    }
}

void ReszvenyMenu::process(){

}
