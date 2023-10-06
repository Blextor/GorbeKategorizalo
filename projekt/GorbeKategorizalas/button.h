#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "common.h"
#include "adatBeolvas.h"
#include "stock.h"

using namespace std;

struct Text {
    int x=0, y=0;
    bool relative = false;
    string str="";

    int r=0, g=0, b=0;

    void setRed(){r=255;g=100;b=100;}
    void setGreen(){g=255;r=100;b=100;}
    void setBlack(){g=0;r=0;b=0;}

    Text(){}
    Text(string s,int vx, int vy, bool rel = false){str=s; x=vx; y=vy; relative=rel;}
    void draw(SDL_Renderer *renderer, int wa, int wb){
        if (!relative)  stringRGBA(renderer,x,y,str.c_str(),r,g,b,255);
        else            stringRGBA(renderer,x+wa,y+wb,str.c_str(),r,g,b,255);
    }
};

struct Button {
    int x=0, y=0;
    int w=0, h=0;
    bool wx=0, wy=0;

    bool afk = false;
    bool selectable = false;
    bool selected = false;

    string str="";

    int X=0, Y=0;

    Button(){}

    Button(string s, int vx, int vy, int vw, int vh, bool vwx, bool vwy){
        str=s; x=vx; y=vy; w=vw; h=vh; wx=vwx; wy=vwy;
    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        if (wx) X = wa + x;
        else X = x;
        if (wy) Y = wb + y;
        else Y = y;

        stringRGBA(renderer,X+5,Y+5,str.c_str(),0,0,0,255);
        rectangleRGBA(renderer,X,Y,X+w+5,Y+h+5,0,0,0,255);
        if (selectable && selected)
            filledCircleRGBA(renderer,X+w-4,Y+h-5,5,100,255,100,255);
        if (selectable)
            circleRGBA(renderer,X+w-4,Y+h-5,5,0,0,0,255);
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

    bool relative = false;

    Gorgetheto(){}
    Gorgetheto(vector<string> e,int vx, int vy, int vw, int vh, int vgx=-1, int vgy=-1){
        elemek=e; x=vx; y=vy; h=vh; w=vw; gridX=vgx; gridY=vgy;
        for (size_t i=0; i<elemek.size(); i++){
            Button temp(elemek[i],0,0,w-25,13,true,true);
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

    void elemekKeresese(string str=""){
        megfeleloElemekSzelekt=megfeleloElemek;
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
        if (relative) {x = wa, y=wb;}
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

        boxRGBA(renderer,x-1,y+2,x+w+5,y+h+5,100,100,100,255);
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

struct KeziGorbe{
    int x=0, y=0;
    string stockName = "";

    int X=0, Y=0;

    Stock *stock;

    int type = 0; /// 0 egy nap - perc bontásban, 1 egy negyedév - nap bontásban
    Datum kezdoNap; /// az a nap, vagy a pénzügyi jelentést követő nyitás napja
    Nap egyetlenNap;    /// a kért nap
    float maxVal = 0, minVal=0;
    Negyed valasztottNegyed; /// a választott negyed
    vector<Arfolyam> gorbe; /// maguk az értékek

    /// kigurás
    Button kiugrasB; /// a kerethez és a "kiugras:" hoz
    Text kiugrasT; /// a kigras mértékéhez

    /// napi nézet
    Button napiB, negyedeviB; /// hogy napról, vagy negyedévről beszélünk
    Button evB, honapB; /// ide lehet beírni, hogy melyik év, melyik hónap
    Button napB; Text napT; /// hogy melyik nap
    vector<string> joDatumok; /// hogy mik lehetnek a napok
    Gorgetheto napok; /// ezeket a jó napokat jelenítsem is meg

    /// negyedéves nézet
    Button negyedevListaB; /// negyedévnek a dátumát kell megadni
    Gorgetheto negyedevListaG; /// lehetséges negyedévek listája


    void gombokElhelyezese(){
        /// kiugrás
        kiugrasB = Button("kiugras:",0,0,125,13,true,true);
        kiugrasT = Text("+41,23%",69,5,true); kiugrasT.setGreen();
        /// napi / negyedév és dátum
        napiB = Button("napi",0,161,51,14,true,true); napiB.selectable=true; napiB.selected=true;
        negyedeviB = Button("negyedev",55,161,81,14,true,true); negyedeviB.selectable=true;
        evB = Button("2023",150,161,36,14,true,true); honapB = Button("10",191,161,21,14,true,true);
        napB = Button("05",217,161,52,14,true,true); napT = Text("V",260,166,true);
    }

    void setLastDay(){
        setGorbeDay((*(stock->mindenNap.rbegin())).datum);
        maxVal=arfolyamGetMaxErtek(gorbe);
        minVal=arfolyamGetMinErtek(gorbe);
        gombokElhelyezese();
    }

    bool setGorbeDay(Datum datum){
        type = 0;
        Nap peldaNap(datum.year,datum.month,datum.day);
        set<Nap>::iterator it = find(stock->mindenNap.begin(),stock->mindenNap.end(),peldaNap);
        if (it==stock->mindenNap.end()) return false;
        egyetlenNap = *it;
        std::vector<Arfolyam> napiErtekek(egyetlenNap.percek.begin(), egyetlenNap.percek.end());
        gorbe.resize(390);
        int j = 0;
        for (int i=0; i<390; i++){
            if (napiErtekek[i+j].idopont<egyetlenNap.idoNyitas) {
                j++; i--;
            } else {
                gorbe[i]=napiErtekek[i+j];
            }
        }
        return true;
    }

    KeziGorbe(Stock *s){
        stock=s;
        setLastDay();

        joDatumok.push_back("01"); joDatumok.push_back("02"); joDatumok.push_back("03"); joDatumok.push_back("04");
        napok = Gorgetheto(joDatumok,0,0,55,100); napok.relative=true;

    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        if (true){
        napok.draw(renderer,wa+214,wb+180);

        /// nagy keret
        rectangleRGBA(renderer,wa,wb,wa+398,wb+180,0,0,0,255);

        /// kiugrás
        kiugrasB.draw(renderer,wa,wb);
        kiugrasT.draw(renderer,wa,wb);

        /// napi/negyed plusz dátum
        napiB.draw(renderer,wa,wb);
        negyedeviB.draw(renderer,wa,wb);

        /// plusz dátum
        evB.draw(renderer,wa,wb);
        honapB.draw(renderer,wa,wb);
        napB.draw(renderer,wa,wb); napT.draw(renderer,wa,wb);
        }

        /// görbe
        stringRGBA(renderer,wa+3,wb+150,"09:30",0,0,0,255);
        stringRGBA(renderer,wa+95,wb+150,"11:00",255,255,255,60);
        stringRGBA(renderer,wa+155,wb+150,"12:00",255,255,255,60);
        stringRGBA(renderer,wa+215,wb+150,"13:00",255,255,255,60);
        stringRGBA(renderer,wa+275,wb+150,"14:00",255,255,255,60);
        stringRGBA(renderer,wa+355,wb+150,"16:00",0,0,0,255);
        rectangleRGBA(renderer,wa,wb+17,wa+398,wb+145,0,0,0,255);
        int yT = 20, yL = 142;

        for (int i=0; i<=4; i++){
            if (i%4!=0){
                stringstream ss; ss<<getPrecFloat((maxVal*(4-i)+minVal*i)/4,4);
                stringRGBA(renderer,wa+2,wb+20+(yL-yT)*i/4-2,ss.str().c_str(),255,255,100,50);
            }
            lineRGBA(renderer,wa+2,wb+20+(yL-yT)*i/4,wa+396,wb+20+(yL-yT)*i/4,255,255,255,50);
        }

        stringstream ss1; ss1<<getPrecFloat(maxVal,4);
        stringstream ss2; ss2<<getPrecFloat(minVal,4);
        stringRGBA(renderer,wa+2,wb+23,ss1.str().c_str(),255,255,100,50);
        stringRGBA(renderer,wa+2,wb+135,ss2.str().c_str(),255,255,100,50);

        for (int i=0; i<gorbe.size(); i++){
            if (gorbe.size()==390){
                int y1 = yL-((gorbe[i].open-minVal)*(yL-yT)/(maxVal-minVal));
                int y2 = yL-((gorbe[i].close-minVal)*(yL-yT)/(maxVal-minVal));
                if (y2<y1) lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,0,255,0,255);
                else lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,255,0,0,255);
            }
            if (i%60==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,30);
            else if (i%30==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,60);

        }

    }

    bool inClick(int bx, int by){

        return false;
    }
};

void loadStock(string name, Stock &stock);
void loadStockOld(string name, Stock &stock);

struct ReszvenySor{
    vector<KeziGorbe> gorbek;
    int x=0, y=0;

    int X=0, Y=0;
    Stock stock;

    std::mutex mfs;
    thread th1;

    Button UjElemB, ElemzesB;

    // Mozgató konstruktor
    ReszvenySor(ReszvenySor&& other) noexcept : stock(other.stock) {
        // A mutex átvétele nem szükséges, mivel az állapota nem befolyásolja az objektum állapotát
    }

    // Mozgató értékadó operátor
    ReszvenySor& operator=(ReszvenySor&& other) noexcept {
        if (this != &other) {
            stock = other.stock;
            // A mutex átvétele itt sem szükséges
        }
        return *this;
    }

    // Töröljük a másoló konstruktort és értékadó operátort
    ReszvenySor(const ReszvenySor&) = delete;
    ReszvenySor& operator=(const ReszvenySor&) = delete;

    ReszvenySor() : x(0) {
        gorbek.clear();
        UjElemB=Button("Uj elem",0,0,60,13,true,true);
        ElemzesB=Button("Elemzes",0,0,60,13,true,true);
    }

    ReszvenySor(int vx, int vy){
        gorbek.clear();
    }



    bool setStock(string name){
        if (!elemeAzStr(osszesReszveny(),name)) return false;
        if (isLocked(mfs)) return false;
        mfs.lock();
        if (th1.joinable()) th1.join();
        th1 = move(thread(loadStock, name, ref(stock)));
        mfs.unlock();
        return true;
    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        rectangleRGBA(renderer, wa, wb, wa+70+gorbek.size()*420, wb+200,0,0,0,255);
        UjElemB.draw(renderer,wa+5,wb+30);
        ElemzesB.draw(renderer,wa+5,wb+60);
        if (!isLocked(mfs))
            stringRGBA(renderer,wa+5,wb+5,stock.name.c_str(),0,0,0,255);
        for (int i=0; i<gorbek.size(); i++){
            gorbek[i].draw(renderer,wa+80+420*i,wb+10);
        }
    }

    bool inClick(int bx, int by){
        if (UjElemB.inClick(bx,by)){
            if (isLocked(mfs)) return true;
            else {
                KeziGorbe temp(&stock);
                gorbek.push_back(temp);
            }
            return true;
        }
        return false;
    }
};

#endif // BUTTON_H_INCLUDED
