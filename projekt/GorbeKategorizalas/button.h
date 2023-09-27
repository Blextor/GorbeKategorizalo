#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "common.h"
#include "adatBeolvas.h"

using namespace std;

struct Text {
    int x=0, y=0;
    string str="";

    Text(){}
    Text(string s,int vx, int vy){str=s; x=vx; y=vy;}
    void draw(SDL_Renderer *renderer, int wa, int wb){
        stringRGBA(renderer,x,y,str.c_str(),0,0,0,255);
    }
};

struct Button {
    int x=0, y=0;
    int w=0, h=0;
    bool wx=0, wy=0;

    bool afk = false;

    string str="";

    int X=0, Y=0;

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
    vector<string> megfeleloElemekSzelekt;
    bool megElem = false;
    bool elemSzelekt = false;
    vector<Button> gombok;
    vector<Button> megjelnoGombok;
    int roll = 0; /// mennyi van görgetve
    int maxRoll = 0; /// meddig tekerhetõ
    int x, y; /// pozíció
    int w, h; /// méret
    int gridX=-1, gridY=-1; /// milyen formába legyenek rendezve

    Gorgetheto(){}
    Gorgetheto(vector<string> e,int vx, int vy, int vw, int vh, int vgx=-1, int vgy=-1){
        elemek=e; x=vx; y=vy; h=vh; w=vw; gridX=vgx; gridY=vgy;
        for (size_t i=0; i<elemek.size(); i++){
            Button temp(elemek[i],0,0,w-25,13,false,false);
            gombok.push_back(temp);
        }
    }

    void elemekFrissitese(vector<string> e, string str=""){
        megfeleloElemekSzelekt=megfeleloElemek;
        elemek=e;
        roll=0;
        gombok.clear();
        for (size_t i=0; i<elemek.size(); i++){
            if (benneVanAzStr(elemek[i],str) || str.size()==0){
                if (!megElem || elemeAzStr(megfeleloElemek,elemek[i])){
                    Button temp(elemek[i],0,0,(w-25-(gridX-1)*00)/max(1,gridX)-8,13,true,true);
                    gombok.push_back(temp);
                }
            }
        }
    }

    void draw(SDL_Renderer *renderer, int wa, int wb){
        int radius = 25;
        int gorgStart = y+13, gorgH = 10, gorgAlsoEnd = y+h-8; // y+h-8
        int gridx = max(1,gridX);//, gridy = max(1,gridY);
        int listLength = (gombok.size()+max(0,gridx-1))/gridx;
        listLength = max(0,14*listLength+6*(listLength));
        maxRoll = max(0,listLength-h+2);
        gorgH = gorgH; /// TODO! (jó lesz ez így)
        int gorgPos=0;
        if (maxRoll>0)
            gorgPos = gorgStart+(gorgAlsoEnd-gorgStart-gorgH)*roll/maxRoll;
        /// gombok
        for (size_t i=0;i<gombok.size();i++) {
            int by = y+6+(i/gridx)*20 - roll;
            if (by+13>=y && by-1<=y+h+5){
                gombok[i].afk=false;
                int tempX = x+3+(i%gridx)*((w-25)/(gridx)+3);
                gombok[i].draw(renderer,tempX,by);
                if (elemSzelekt){
                    if (!elemeAzStr(megfeleloElemekSzelekt,gombok[i].str))
                        filledCircleRGBA(renderer,tempX+gombok[i].w-5,by+8,5,80,80,80,255);
                    else
                        filledCircleRGBA(renderer,tempX+gombok[i].w-5,by+8,5,80,200,80,255);
                    circleRGBA(renderer,tempX+gombok[i].w-5,by+8,5,0,0,0,255);
                }
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
        for (size_t i=0; i<gombok.size(); i++){
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

struct ProgressBar{
    int x=0, y=0;
    int w=0, h=0;
    bool wx=0, wy=0;

    int X=0,Y=0;

    bool stopped = true;
    bool cancelled = true;

    int eta_hour=0, eta_min=0, eta_sec=0;
    int feldolgozottElemek=0, osszesElem=0;
    float keszsegSzazalek=0;

    string holAllEpp;

    clock_t startTime = 1;
    clock_t forDeltaTime = 1;
    clock_t timePassed = 1;

    //mutex rajzolasi;

    ProgressBar(){}
    ProgressBar(int vx, int vy, int vw, int vh, bool vwx, bool vwy){
        x=vx; y=vy; w=vw; h=vh; wx=vwx; wy=vwy;
    }

    void elemFeldolgozva(int cnt=1){

        if (cnt<0){
            osszesElem+=cnt;
            if(osszesElem<1) osszesElem=1;
            forDeltaTime=clock();
        } else if (cnt>0){
            feldolgozottElemek+=cnt;
            if(osszesElem<feldolgozottElemek)
                feldolgozottElemek=osszesElem;
        } else {

            stop();
        }
    }

    void prepare(int oE){
        cancelled=true;
        osszesElem=oE;
    }

    void start(){
        if (cancelled)
            feldolgozottElemek=0;
        cancelled=false;
        stopped=false;
        startTime=clock();
        forDeltaTime=clock();
    }

    void stop(){
        stopped=true;
    }

    void cancel(){
        stop();
        cancelled=true;
        osszesElem=0;
        feldolgozottElemek=0;
        timePassed=1;
        holAllEpp="";
    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        if (!stopped && !cancelled){
            clock_t t = clock();
            timePassed+=(t-forDeltaTime);
            forDeltaTime=t;
            if (t-forDeltaTime>5000) timePassed-=(t-forDeltaTime);
        }
        long unsigned int i = timePassed/1000;

        if (wx) X = wa - x;
        else X = x;
        if (wy) Y = wb - y;
        else Y = y;

        rectangleRGBA(renderer,X,Y,X+w,Y+h,0,0,0,255);
        float p = (float)feldolgozottElemek/(float)osszesElem;
        int kti = p*100*100;
        float ktp = (float)kti/100.0f;

        if (p>0)
            boxRGBA(renderer,X+1,Y+1,X+1+((w-3)*p),Y+h-2,100-50*p,145+60*p,100-50*p,255);
        if (p>=1)
            boxRGBA(renderer,X+1,Y+1,X+1+((w-3)*p),Y+h-2,0,255,0,255);

        int tempF = max(feldolgozottElemek,1);
        float tempO = osszesElem;
        i = i*(tempO-tempF)/tempF;
        eta_hour = i/3600;
        eta_min = (i%3600)/60;
        eta_sec = i%60;
        if (cancelled){eta_hour=0; eta_min=0; eta_sec=0;}
        stringstream ss1; ss1<<"ETA: ";
        if (eta_hour>0) ss1<<eta_hour<<":";
        if (eta_min>0 || eta_hour>0) ss1<<eta_min/10<<eta_min%10<<":";
        ss1<<eta_sec/10<<eta_sec%10;

        stringRGBA(renderer,X+5,Y+h+7,ss1.str().c_str(),0,0,0,255);

        stringstream ss2;
        if (cancelled)
            ss2<<"_ / _ - _ %";
        else
            ss2<<feldolgozottElemek<<" / "<<osszesElem<<" - "<<ktp<<" %";

        stringRGBA(renderer,X+w/2-120,Y+h+7,ss2.str().c_str(),0,0,0,255);

        if (osszesElem==feldolgozottElemek && osszesElem!=0)
            stringRGBA(renderer,X+w-35,Y+h+7,"kesz",0,0,0,255);
    }

};

#endif // BUTTON_H_INCLUDED
