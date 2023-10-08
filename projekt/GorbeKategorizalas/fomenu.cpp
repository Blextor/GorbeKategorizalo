#include "Menu.h"

void FoMenu::gombokKialakitasa(){
    /// fejl�c sz�vegek
    FoCim = Text("Gorbe kategorizalo",100,100);

    /// men� v�lt� gombok
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
    int x, y; /// ablak m�retei
    SDL_GetWindowSize(window,&x,&y); /// m�retek lek�rdez�se
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
    int MX=-1, MY=-1; /// kurzor poz�ci�ja, ha -1 marad, nem t�rt�nt v�ltoz�s
    //bool leftButton = true; /// k�l�n k�gy�jt�m, hogy lenyomt�k-e a bal eg�rgombot
    bool keyDown = false; /// vagy b�rmelyt a billenty�zeten
    //bool mouseWheel = false; /// vagy g�rgettek-e

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
            ///mouseWheel=true;
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
        if (FrissMB.inClick(MX,MY)) *menu = frissitoMenu; /// f�men�be �r�ny�t� gombot
        else if (RCSMB.inClick(MX,MY)) *menu = csoportEditormenu; /// csoport szerkszt� men� gombot
        else if (GorbeMB.inClick(MX,MY)) *menu = gorbeMenu; /// r�szv�ny men� gombot
        else if (KilepB.inClick(MX,MY)) exit(3); /// csoport szerkszt� men� gombot
        else if (szalMennyisegB.inClick(MX,MY)) {
            state=1;
        }

        else { /// ha kikattintunk a semmibe
            state=0; /// akkor t�rj�nk vissza a kezd� �llapotba
        }

    }

    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){ /// bevitel
        timestampText=ev->text.timestamp; /// sz�vegbevitel esem�ny v�gtelen, de id�b�lyege nem
        if (state==1){ /// �j r�szv�ny
            if (isalnum(ev->text.text[0]) && !isalpha(ev->text.text[0])){ /// csak karakterek lehetnek
                szalMennyisegB.str+=ev->text.text[0];
            }
        }

    }

    if (keyDown){ /// ha billenty�zetet nem g�pel�s miatt �t�tt�k le
        if (ev->key.keysym.sym==SDLK_BACKSPACE){ /// pl. t�rl�s miatt
            /// el�z� beviteli mez�k tartalm�nak reduk�l�sa �s sz�r�sek friss�t�se
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


