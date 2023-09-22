#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "common.h"
#include "adatBeolvas.h"

using namespace std;

struct Text {
    int x, y;
    string str;

    Text(){}
    Text(string s,int vx, int vy){str=s; x=vx; y=vy;}
    void draw(SDL_Renderer *renderer, int wa, int wb){
        stringRGBA(renderer,x,y,str.c_str(),0,0,0,255);
    }
};

struct Button {
    int x, y;
    int w, h;
    bool wx, wy;

    bool afk = false;

    string str;

    int X, Y;

    Button(){}

    Button(string s, int vx, int vy, int vw, int vh, bool vwx, bool vwy){
        str=s; x=vx; y=vy; w=vw; h=vh; wx=vwx; wy=vwy;
    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        if (wx) X = wa - x;
        else X = x;
        if (wy) Y = wb - y;
        else Y = y;

        stringRGBA(renderer,X+5,Y+5,str.c_str(),0,0,0,255);
        rectangleRGBA(renderer,X,Y,X+w+5,Y+h+5,0,0,0,255);
    }

    bool inClick(int bx, int by){
        return X<=bx && bx<=X+w+5 && Y<=by && by<=Y+h+5;
    }
};

struct Gorgetheto {
    vector<string> elemek;
    vector<string> megfeleloElemek;
    bool megElem = false;
    vector<Button> gombok;
    vector<Button> megjelnoGombok;
    int roll = 0; /// mennyi van görgetve
    int maxRoll = 0; /// meddig tekerhetõ
    int x, y; /// pozíció
    int w, h; /// méret
    int gridX=-1, gridY=-1; /// milyen formába legyenek rendezve

    Gorgetheto(){}
    Gorgetheto(vector<string> e,int vx, int vy, int vw, int vh, int vgx=-1, int vgy=-1)
    {
        elemek=e; x=vx; y=vy; h=vh; w=vw; gridX=vgx; gridY=vgy;
        for (int i=0; i<elemek.size(); i++){
            Button temp(elemek[i],0,0,w-25,13,false,false);
            gombok.push_back(temp);
        }
    }

    void elemekFrissitese(vector<string> e, string str=""){
        elemek=e;
        roll=0;
        gombok.clear();
        for (int i=0; i<elemek.size(); i++){
            if (benneVanAzStr(elemek[i],str) || str.size()==0){
                if (!megElem || elemeAzStr(megfeleloElemek,elemek[i])){
                    Button temp(elemek[i],0,0,(w-25)/max(1,gridX),13,true,true);
                    gombok.push_back(temp);
                }
            }
        }
    }

    void draw(SDL_Renderer *renderer, int wa, int wb){
        int radius = 25;
        int gorgStart = y+13, gorgH = 10, gorgAlsoEnd = y+h-8; // y+h-8
        int listLength = gombok.size();
        listLength = max(0,14*listLength+6*(listLength));
        maxRoll = max(0,listLength-h+2);
        gorgH = gorgH; /// TODO! (jó lesz ez így)
        int gorgPos=0;
        int gridx = max(1,gridX), gridy = max(1,gridY);
        if (maxRoll>0)
            gorgPos = gorgStart+(gorgAlsoEnd-gorgStart-gorgH)*roll/maxRoll;
        /// gombok
        for (int i=0;i<gombok.size();i++) {
            int by = y+6+(i/gridx)*20 - roll;
            if (by+13>=y && by-1<=y+h+5){
                gombok[i].afk=false;
                gombok[i].draw(renderer,x+3+(i%gridx)*(w/gridx)+max(0,(i%gridx))*10,by);
            }
            else gombok[i].afk=true;
        }
        /// görgõ
        if (maxRoll>0){
            rectangleRGBA(renderer,x+w-9,y+11,x+w-3,y+h-5,50,50,50,255);
            boxRGBA(renderer,x+w-11,gorgPos,x+w-2,gorgPos+gorgH,50,rollSpeed%255,50,255);
        }
        /// elfedõ
        boxRGBA(renderer,x-radius,y-radius,x+w+radius,y+2,100,100,100,255);
        boxRGBA(renderer,x-radius,y-radius,x-1,y+h+radius,100,100,100,255);
        boxRGBA(renderer,x+w+5,y-radius,x+w+radius,y+h+radius,100,100,100,255);
        boxRGBA(renderer,x-radius,y+h+5,x+w+radius,y+h+radius,100,100,100,255);
        rectangleRGBA(renderer,x,y+3,x+w+5,y+h+5,0,0,0,255);

    }

    bool inClick(int bx, int by){
        if (bx<=x+w+4 && x<=bx && by<=y+h+4 && y<=by) {

            return true;
        }
        return false;
    }

    string whichButton(int bx, int by){
        for (int i=0; i<gombok.size(); i++){
            if (!gombok[i].afk && gombok[i].inClick(bx,by)){
                return gombok[i].str;
            }
        }
        return "";
    }

    int rollSpeed = 5;

    void rollIt(int c){
        roll+=c*rollSpeed;
        if (roll<0) roll=0;
        if (roll>maxRoll) roll=maxRoll;
    }

    void speedUpRoll(){rollSpeed*=2; if (rollSpeed>5*16) rollSpeed=5*16;}
    void speedDownRoll(){rollSpeed/=2; if (rollSpeed<5) rollSpeed=5;}
};

#endif // BUTTON_H_INCLUDED
