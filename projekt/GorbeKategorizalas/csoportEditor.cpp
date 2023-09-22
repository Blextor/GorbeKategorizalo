#include "Menu.h"


void CsoportEditorMenu::gombokKialakitasa(){
    /// Menü gombok
    FMB = Button("FoMenube",15,40,70,13,false,true);
    AFMB = Button("Adatok frissitese",110,40,141,13,false,true);
    /// Új elem megadása
    MegCimTxt = Text("Uj reszveny vagy csoport letrehozasa", 20,35);
    ReszMegB = Button("uj reszveny",40,60,124,13,false,false);
    CsoportMegB = Button("uj csoport neve",40,90,124,13,false,false);
    ReszMegPluszB = Button("+",175,60,13,13,false,false);
    CsoportMegPluszB = Button("+",175,90,13,13,false,false);
    ReszMegTxt = Text("", 200,65);
    CsoportMegTxt = Text("", 200,95);
    /// Meglévõ elem törlése
    MinCimTxt = Text("Meglevo reszveny vagy csoport torlese", 330,35);
    ReszMinB = Button("reszveny neve V",350,60,124,13,false,false);
    CsoportMinB = Button("csoport neve  V",350,90,124,13,false,false);
    ReszMinG = Gorgetheto(meglevoReszvenyek,350,75,124,102);
    CsoportMinG = Gorgetheto(meglevoCsoportok,350,105,124,102);
    ReszMinMinB = Button("-",485,60,13,13,false,false);
    CsoportMinMinB = Button("-",485,90,13,13,false,false);
    ReszMinTxt = Text("", 510,65);
    CsoportMinTxt = Text("", 510,95);
    /// Kategória kiválasztása és szerkesztése
    OsszResz = Gorgetheto(meglevoReszvenyek,5,240,400,200,5);
    AktCsop = Button("",460,240,124,13,false,false);
    AktCsopOk = Button("OK",600,240,20,13,false,false);
    OsszCsop = Gorgetheto(meglevoCsoportok,460,260,124,102);
    CsopValTxt = Text("Valaszott csoport:",456,225);
}

void CsoportEditorMenu::nextMenus(Menu *fo, Menu *friss){
    fomenu=fo;
    frissitomenu=friss;

    gombokKialakitasa();
}

void CsoportEditorMenu::draw() {

    int x, y;
    SDL_GetWindowSize(window,&x,&y);
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderClear(renderer);

    stringRGBA(renderer,10,10,"Reszvenyek es csoportok fejlesztese",0,0,0,255);
    lineRGBA(renderer,320,20,320,110,0,0,0,255);
    //rectangleRGBA(renderer,5,145,120,161,0,0,0,255);

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

    if (true){
    OsszResz.draw(renderer,x,y);
    OsszCsop.draw(renderer,x,y);
    AktCsop.draw(renderer,x,y);
    AktCsopOk.draw(renderer,x,y);
    CsopValTxt.draw(renderer,x,y);
    stringRGBA(renderer,10,220,"Reszvenyek:",0,0,0,255);
    }


    SDL_RenderPresent(renderer);
}

void CsoportEditorMenu::inputHandle() {
    int MX=-1, MY=-1;
    bool leftButton = true;
    bool keyDown = false;
    bool mouseWheel = false;
    if (SDL_PollEvent(ev)){
        if (ev->type==SDL_MOUSEBUTTONDOWN){
            MX=ev->button.x;
            MY=ev->button.y;
            leftButton=ev->button.button==SDL_BUTTON_LEFT;
            cout<<MX<<" "<<MY<<" "<<leftButton<<endl;
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        }
        if (ev->type==SDL_MOUSEWHEEL){
            mouseWheel=true;
        }
        if (ev->type==SDL_KEYDOWN){
            keyDown=true;
            if (ev->key.keysym.sym==SDLK_r){
                cout<<"CsoportEditorMenu"<<endl;
            }
        }
        if (ev->type == SDL_QUIT)
            exit(3);
    }
    if (MX!=-1){
        if (FMB.inClick(MX,MY)) *menu = fomenu;
        else if (AFMB.inClick(MX,MY)) *menu = frissitomenu;
        else if (ReszMegB.inClick(MX,MY)) {
            ReszMegB.str="";
            ReszMegTxt.str="";
            state = 1;
        }
        else if (CsoportMegB.inClick(MX,MY)) {
            CsoportMegB.str="";
            CsoportMegTxt.str="";
            state = 2;
        }
        else if (ReszMegPluszB.inClick(MX,MY)) {
            if (state==1){
                if (ReszMegB.str.size()==0) ReszMegTxt.str = "Ures!";
                else ReszMegTxt.str = createNewReszveny(ReszMegB.str);
            }
            ReszMegB.str="uj reszveny";
            state = 0;
        }
        else if (CsoportMegPluszB.inClick(MX,MY)) {
            if (state==2){
                if (CsoportMegB.str.size()==0) CsoportMegTxt.str = "Ures!";
                else CsoportMegTxt.str = createNewCsoport(CsoportMegB.str);
            }
            CsoportMegB.str="uj csoport neve";
            state = 0;
        }
        else if (ReszMinB.inClick(MX,MY)){
            ReszMinTxt.str="";
            if (state!=3){
                ReszMinB.str="";
                ReszMinG.elemekFrissitese(meglevoReszvenyek);
            }
            state=3;
        }
        else if (state == 3 && ReszMinG.inClick(MX,MY)){
            string btStr = ReszMinG.whichButton(MX,MY);
            if (btStr!=""){
                ReszMinB.str=btStr;
                ReszMinG.elemekFrissitese(meglevoReszvenyek,btStr);
            } else {

            }
        }
        else if (state == 3 && ReszMinMinB.inClick(MX,MY)){
            state=0;
            if (ReszMinB.str.size()==0){
                ReszMinTxt.str="Ures!";
            } else
                ReszMinTxt.str=deleteReszveny(ReszMinB.str);
        }
        else if (CsoportMinB.inClick(MX,MY)){
            CsoportMinTxt.str="";
            if (state!=4){
                CsoportMinB.str="";
                CsoportMinG.elemekFrissitese(meglevoCsoportok);
            }
            state=4;
        }
        else if (state == 4 && CsoportMinG.inClick(MX,MY)){
            string btStr = CsoportMinG.whichButton(MX,MY);
            if (btStr!=""){
                CsoportMinB.str=btStr;
                CsoportMinG.elemekFrissitese(meglevoCsoportok,btStr);
            } else {

            }
        }
        else if (state == 4 && CsoportMinMinB.inClick(MX,MY)){
            state=0;
            if (CsoportMinB.str.size()==0){
                CsoportMinTxt.str="Ures!";
            } else
                CsoportMinTxt.str=deleteCsoport(CsoportMinB.str);
        }
        else if (AktCsop.inClick(MX,MY)){
            if (state!=5)
                OsszCsop.elemekFrissitese(meglevoCsoportok,"");
            state = 5;
        } else if (state == 5 && OsszCsop.inClick(MX,MY)){
            string btStr = OsszCsop.whichButton(MX,MY);
            if (btStr!=""){
                AktCsop.str=btStr;
                OsszCsop.elemekFrissitese(meglevoCsoportok,btStr);
            } else {

            }
        } else if (state == 5 && AktCsopOk.inClick(MX,MY)){
            if (elemeAzStr(meglevoCsoportok,AktCsop.str)){
                aktCsopStr=AktCsop.str;
                aktualisCsoportListaja=csoportReszvenyei(aktCsopStr);
                OsszResz.megfeleloElemek=aktualisCsoportListaja;
                OsszResz.megElem=true;
            } else {
                aktCsopStr="";
                OsszResz.megfeleloElemek.clear();
                aktualisCsoportListaja.clear();
                OsszResz.megElem=false;
            }
            OsszResz.elemekFrissitese(meglevoReszvenyek);
            state = 0;
        }
        else {
            state=0;
        }
        cout<<"state "<<state<<endl;
    }
    if (ev->type == SDL_TEXTINPUT && ev->text.timestamp!=timestampText){
        timestampText=ev->text.timestamp;
        if (state==1){
            if (isalpha(ev->text.text[0]))
                ReszMegB.str+=ev->text.text[0];
        }
        if (state==2){
            if (isalpha(ev->text.text[0]))
                CsoportMegB.str+=ev->text.text[0];
        }
        if (state==3){
            if (isalpha(ev->text.text[0])){
                ReszMinB.str+=ev->text.text[0];
                ReszMinG.elemekFrissitese(meglevoReszvenyek,ReszMinB.str);
            }
        }
        if (state==4){
            if (isalpha(ev->text.text[0])){
                CsoportMinB.str+=ev->text.text[0];
                CsoportMinG.elemekFrissitese(meglevoCsoportok,CsoportMinB.str);
            }
        }
        if (state==5){
            if (isalpha(ev->text.text[0])){
                AktCsop.str+=ev->text.text[0];
                OsszCsop.elemekFrissitese(meglevoCsoportok,AktCsop.str);
            }
        }
        //cout<<ev->text.text[0] << " "<<ev->text.timestamp<<endl;
    }
    if (keyDown){
        if (ev->key.keysym.sym==SDLK_BACKSPACE){
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
        if (ev->key.keysym.sym==SDLK_UP){
            if (state==3) {ReszMinG.speedUpRoll();}
            if (state==4) {CsoportMinG.speedUpRoll();}
            if (state==5) {OsszCsop.speedUpRoll();}
        }
        if (ev->key.keysym.sym==SDLK_DOWN){
            if (state==3) {ReszMinG.speedDownRoll();}
            if (state==4) {CsoportMinG.speedDownRoll();}
            if (state==5) {OsszCsop.speedDownRoll();}
        }
    }
    if (mouseWheel){
        if (state==3)ReszMinG.rollIt(-ev->wheel.y);
        if (state==4)CsoportMinG.rollIt(-ev->wheel.y);
        if (state==5)OsszCsop.rollIt(-ev->wheel.y);
    }
}

void CsoportEditorMenu::process(){
    meglevoReszvenyek = osszesReszveny();
    meglevoCsoportok = osszesCsoport();
    if (firstInitOszzLista){
        firstInitOszzLista=false;
        OsszResz.elemekFrissitese(meglevoReszvenyek);
    }

    if (oldState!=state){
        oldState=state;
        if (state!=1){
            ReszMegB.str="uj reszveny";
        }
        if (state!=2){
            CsoportMegB.str="uj csoport neve";
        }
        if (state!=3){
            ReszMinB.str="reszveny neve V";
            ReszMinG.roll=0;
        }
        if (state!=4){
            CsoportMinB.str="csoport neve  V";
            CsoportMinG.roll=0;
        }
        if (state!=5){
            AktCsop.str=aktCsopStr;
            OsszCsop.roll=0;
            OsszCsop.elemekFrissitese(meglevoCsoportok,"-1");
        }
    }

    if (!kiszamoltState){
        if (state==0){

        }
    }
}

