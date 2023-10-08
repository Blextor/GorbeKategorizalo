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

    int r=0, g=0, b=0, a=255;

    void setRed(){r=255;g=100;b=100;a=255;}
    void setGreen(){g=255;r=100;b=100;a=255;}
    void setBlack(){g=0;r=0;b=0;a=255;}
    void setGold(){r=255;g=255;b=100;a=50;}
    void setGray(){r=255;g=255;b=255;a=60;}

    Text(){}
    Text(string s,int vx, int vy, bool rel = false){str=s; x=vx; y=vy; relative=rel;}
    void draw(SDL_Renderer *renderer, int wa, int wb){
        if (!relative)  stringRGBA(renderer,x,y,str.c_str(),r,g,b,a);
        else            stringRGBA(renderer,x+wa,y+wb,str.c_str(),r,g,b,a);
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
    float maxVal = 0, minVal=0, nullVal=0, elozoZarVal = 0;
    Negyed valasztottNegyed; /// a választott negyed
    vector<Arfolyam> gorbe; /// maguk az értékek

    /// kigurás
    Button kiugrasB; /// a kerethez és a "kiugras:" hoz
    Text kiugrasT; /// a kigras mértékéhez

    /// napi nézet
    Button napiB, negyedeviB; /// hogy napról, vagy negyedévről beszélünk
    Button evB, honapB; /// ide lehet beírni, hogy melyik év, melyik hónap
    Button napB; Text napT; /// hogy melyik nap + napoknak a görgéséért felelős lenyíl
    vector<string> joDatumok; /// hogy mik lehetnek a napok
    Gorgetheto napok; /// ezeket a jó napokat jelenítsem is meg

    /// negyedéves nézet
    Button negyedevB; /// negyedévnek a dátumát kell megadni
    Text negyedevT; /// negyedévnek a görgéséért felelős lenyíl
    vector<string> joDatumok2; /// hogy mik lehetnek a negyedévek
    Gorgetheto negyedevG; /// lehetséges negyedévek listája

    /// kiválaszott dátum OK gomb
    Button datumOKB;

    /// következő és előző dátum gomb a léptetéshez
    Button kovetkezoDatumB, elozoDatumB;

    /// százalékos eltérés gomb, és előző nap/negyedévi záráshoz képest
    Button szazalekB, elozohozB;

    /// kiírt értékek
    vector<Text> reszvenyErtekek;
    vector<Text> reszvenyDatumok;
    vector<Text> reszvenyIdopontok;


    void gombokElhelyezese(){
        /// kiugrás
        kiugrasB = Button("kiugras:",0,0,117,13,true,true);
        kiugrasT = Text("",69,5,true); kiugrasT.setGreen();
        /// napi / negyedév és dátum
        napiB = Button("napi",0,161,51,13,true,true); napiB.selectable=true; napiB.selected=true;
        negyedeviB = Button("negyedev",55,161,81,13,true,true); negyedeviB.selectable=true;
        evB = Button("",150,161,36,13,true,true); honapB = Button("",191,161,21,13,true,true);
        napB = Button("",217,161,52,13,true,true); napT = Text("V",260,166,true);
        //napok = Gorgetheto(joDatumok,214,180,55,50); napok.relative=true;

        negyedevB = Button("",150,161,110,13,true,true);
        negyedevT = Text("V",251,166,true);
        //negyedevG = Gorgetheto(joDatumok2,150,179,113,50); negyedevG.relative=true;

        datumOKB = Button("OK", 290,161,20,13,true,true);

        if (true){ /// reszvenyErtekek
        reszvenyErtekek.resize(5);
        reszvenyErtekek[0] = Text("",2,23,true); reszvenyErtekek[0].setGold();
        reszvenyErtekek[1] = Text("",2,49,true); reszvenyErtekek[1].setGold();
        reszvenyErtekek[2] = Text("",2,80,true); reszvenyErtekek[2].setGold();
        reszvenyErtekek[3] = Text("",2,110,true); reszvenyErtekek[3].setGold();
        reszvenyErtekek[4] = Text("",2,135,true); reszvenyErtekek[4].setGold();
        }
        if (true){ /// reszvenyIdopontok
            reszvenyIdopontok.resize(6);
            reszvenyIdopontok[0] = Text("09:30",3,150,true); reszvenyIdopontok[0].setBlack();
            reszvenyIdopontok[1] = Text("11:00",95,150,true); reszvenyIdopontok[1].setGray();
            reszvenyIdopontok[2] = Text("12:00",155,150,true); reszvenyIdopontok[2].setGray();
            reszvenyIdopontok[3] = Text("13:00",215,150,true); reszvenyIdopontok[3].setGray();
            reszvenyIdopontok[4] = Text("14:00",275,150,true); reszvenyIdopontok[4].setGray();
            reszvenyIdopontok[5] = Text("16:00",355,150,true); reszvenyIdopontok[5].setBlack();
        }

        szazalekB = Button("%",282,0,25,13,true,true); szazalekB.selectable=true;
        elozohozB = Button("elozohoz",312,0,81,13,true,true); elozohozB.selectable=true;

        kovetkezoDatumB = Button("->",365,161,20,13,true,true);
        elozoDatumB = Button("<-",340,161,20,13,true,true);
    }

    void setNextNap(){
        Datum datum = egyetlenNap.kovetkezoNap;
        if (datum.year==-1) return;
        Nap kovetkezoNap(datum);
        vector<string> strT = datum.toString();
        evB.str=strT[0]; honapB.str=strT[1]; napB.str=strT[2];
        egyetlenNap = (*(stock->mindenNap.find(kovetkezoNap)));
    }

    void setNextNegyed(){
        set<Negyed>::iterator it = stock->negyedevek.find(valasztottNegyed);
        if (++it==stock->negyedevek.end()) return;
        Datum d = it->idoszakVege;
        vector<string> strT = d.toString();
        negyedevB.str = (strT[0]+" "+strT[1]+" "+strT[2]);
        valasztottNegyed = *it;
    }

    void setBeforeNap(){
        Datum datum = egyetlenNap.elozoNap;
        if (datum.year==-1) return;
        Nap elozoNap(datum);
        vector<string> strT = datum.toString();
        evB.str=strT[0]; honapB.str=strT[1]; napB.str=strT[2];
        egyetlenNap = (*(stock->mindenNap.find(elozoNap)));
    }

    void setBeforeNegyed(){
        set<Negyed>::iterator it = stock->negyedevek.find(valasztottNegyed);
        if (it==stock->negyedevek.begin()) return;
        --it;
        Datum d = it->idoszakVege;
        vector<string> strT = d.toString();
        negyedevB.str = (strT[0]+" "+strT[1]+" "+strT[2]);
        valasztottNegyed = *it;
    }

    void setLastNap(){
        egyetlenNap = (*(stock->mindenNap.rbegin()));
        vector<string> datum = egyetlenNap.datum.toString();
        evB.str=datum[0];
        honapB.str=datum[1];
        napB.str=datum[2];
    }

    void setLastNegyed(){
        valasztottNegyed = (*(stock->negyedevek.rbegin()));
        vector<string> datum = valasztottNegyed.idoszakVege.toString();
        string temp = datum[0] + " " + datum[1] + " " + datum[2];
        negyedevB.str = temp;
    }

    bool setGorbeNegyed(Datum datum){
        type = 1;
        Negyed peldaNegyed(datum.year,datum.month,datum.day);
        set<Negyed>::iterator it = find(stock->negyedevek.begin(),stock->negyedevek.end(),peldaNegyed);
        if (it==stock->negyedevek.end()) return false;
        valasztottNegyed = *it;
        std::vector<Arfolyam> napErtekek; //(egyetlenNap.percek.begin(), egyetlenNap.percek.end());
        gorbe.clear();

        Nap kezdoNap(valasztottNegyed.korrigaltTenylegesJelentes);
        auto it2 = stock->mindenNap.find(kezdoNap);
        while ((*it2).datum<valasztottNegyed.negyedevVege){
            Nap nap = *it2;
            napErtekek.push_back(Arfolyam(nap.datum.month,nap.datum.day,nap.nyitas,nap.zaras,nap.minimum,nap.maximum,nap.volumen));
            ++it2;
            if (it2 == stock->mindenNap.end()) break;
        }

        gorbe = napErtekek;
        return true;
    }

    bool setGorbeNap(Datum datum){
        type = 0;
        Nap peldaNap(datum.year,datum.month,datum.day);
        set<Nap>::iterator it = find(stock->mindenNap.begin(),stock->mindenNap.end(),peldaNap);
        if (it==stock->mindenNap.end()) return false;
        egyetlenNap = *it;
        std::vector<Arfolyam> napiErtekek(egyetlenNap.percek.begin(), egyetlenNap.percek.end());
        gorbe.clear();
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

    void pushGorbeElozoNap(Datum most){
        Nap peldaNap(most.year,most.month,most.day);
        set<Nap>::iterator it = find(stock->mindenNap.begin(),stock->mindenNap.end(),peldaNap);
        if (it==stock->mindenNap.end()) return;
        Nap elozoNap((*it).elozoNap);
        set<Nap>::iterator it2 = find(stock->mindenNap.begin(),stock->mindenNap.end(),elozoNap);
        if (it2==stock->mindenNap.end()) return;
        Arfolyam temp(1,1,(*it2).zaras,(*it2).zaras,(*it2).zaras,(*it2).zaras,(*it2).volumen);
        gorbe.push_back(temp);
    }

    void setGorbeErtekek(){
        bool szazalek = szazalekB.selected;
        /// reszvenyErtekek
        float maxValV=max(maxVal,nullVal);
        float minValV=min(minVal,nullVal);
        if (szazalek){
            maxValV=(maxValV/nullVal-1.0f)*100;
            minValV=(1.0f-nullVal/minValV)*100;
        }
        for (size_t i=0; i<reszvenyErtekek.size(); i++){
            float ertek=(maxValV*(reszvenyErtekek.size()-i-1)+
                                minValV*i)/(reszvenyErtekek.size()-1);
            ertek = getPrecFloat(ertek,4);
            stringstream ss; ss<<ertek; if (szazalek) ss<<"%";
            string temp; ss>>temp;
            reszvenyErtekek[i].str=temp;
        }

        /// kiugrás értékét és színét beállítja
        float kiugras;
        if (elozoZarVal<gorbe[0].open)  kiugras = gorbe[0].open/elozoZarVal - 1.0f;
        else                            kiugras = 1.0f - elozoZarVal/gorbe[0].open;
        kiugras=getPrecFloat(kiugras*100,3);
        stringstream ss; if (kiugras>=0) ss<<"+";
        ss<<kiugras<<"%";
        kiugrasT.str=ss.str();
        if (kiugras>=0) kiugrasT.setGreen();
        else kiugrasT.setRed();
    }

    void gorbeFrissitese(){
        bool elozohoz = elozohozB.selected;
        bool napi = napiB.selected;
        Datum datum;
        bool ujAdatok = false;
        if (napi) {
            stringstream ss; ss<<evB.str<<" "<<honapB.str<<" "<<napB.str;
            int tempE, tempH, tempN;
            ss>>tempE>>tempH>>tempN;
            datum=Datum(tempE,tempH,tempN);
            ujAdatok=setGorbeNap(datum);
            if (ujAdatok) pushGorbeElozoNap(datum);
        } else {
            stringstream ss(negyedevB.str);
            int tempE, tempH, tempN;
            ss>>tempE>>tempH>>tempN;
            datum=Datum(tempE,tempH,tempN);
            ujAdatok=setGorbeNegyed(datum);
            if (ujAdatok) pushGorbeElozoNap(valasztottNegyed.korrigaltTenylegesJelentes);
        }
        if (!ujAdatok) {
            vector<string> strT = valasztottNegyed.idoszakVege.toString();
            negyedevB.str = (strT[0]+" "+strT[1]+" "+strT[2]);
            strT = egyetlenNap.datum.toString();
            evB.str = strT[0];
            honapB.str = strT[1];
            napB.str = strT[2];
            return;
        }
        elozoZarVal = gorbe[gorbe.size()-1].close;
        if (!elozohoz){
            nullVal=gorbe[0].open;
            gorbe.pop_back();
        }
        maxVal=arfolyamGetMaxErtek(gorbe);
        minVal=arfolyamGetMinErtek(gorbe);
        if (elozohoz) {
            nullVal=gorbe[gorbe.size()-1].close;
            gorbe.pop_back();
        }

        setGorbeErtekek();
    }

    KeziGorbe(Stock *s){
        //joDatumok.push_back("01"); joDatumok.push_back("02"); joDatumok.push_back("03"); joDatumok.push_back("04");
        //joDatumok2.push_back("2023 10 04"); joDatumok2.push_back("2023 10 05"); joDatumok2.push_back("2023 10 06"); joDatumok2.push_back("2023 10 07");
        gombokElhelyezese();
        stock=s;
        setLastNap();
        gorbeFrissitese();

    }

    void draw (SDL_Renderer *renderer, int wa, int wb){
        if (true){
        /// jelenleg nem kellenek a görgethető dátumok
        //if (type==0) napok.draw(renderer,wa+214,wb+180);
        //if (type==1) negyedevG.draw(renderer,wa+147,wb+179);

        /// nagy keret
        rectangleRGBA(renderer,wa,wb,wa+398,wb+179,0,0,0,255);

        /// kiugrás
        kiugrasB.draw(renderer,wa,wb);
        kiugrasT.draw(renderer,wa,wb);

        /// napi/negyed plusz dátum
        napiB.draw(renderer,wa,wb);
        negyedeviB.draw(renderer,wa,wb);

        /// plusz dátum és OK
        if (type==0){
            evB.draw(renderer,wa,wb);
            honapB.draw(renderer,wa,wb);
            napB.draw(renderer,wa,wb); napT.draw(renderer,wa,wb);
        }

        if (type==1){
            negyedevB.draw(renderer,wa,wb);
            negyedevT.draw(renderer,wa,wb);
        }
        datumOKB.draw(renderer,wa,wb);
        /// léptető gombok
        kovetkezoDatumB.draw(renderer,wa,wb);
        elozoDatumB.draw(renderer,wa,wb);
        }

        /// előző napot figyelembe véve, és százalékos megjelenítés
        elozohozB.draw(renderer,wa,wb);
        szazalekB.draw(renderer,wa,wb);

        /// görbe időpontok / dátumok
        for (int i=0; i<6; i++){ reszvenyIdopontok[i].draw(renderer,wa,wb); }
        rectangleRGBA(renderer,wa,wb+17,wa+398,wb+145,0,0,0,255);

        /// vízszintes vonalak és értékek
        int yT = 20, yL = 142;
        for (int i=0; i<=4; i++){
            reszvenyErtekek[i].draw(renderer,wa,wb);
            lineRGBA(renderer,wa+2,wb+20+(yL-yT)*i/4,wa+396,wb+20+(yL-yT)*i/4,255,255,255,50);
        }

        /// a részvény értékei, ez a saját megvalósítás, így meghagyom így
        for (int i=0; i<gorbe.size(); i++){
            if (gorbe.size()==390){ /// egy napról van szó
                int y1 = yL-((gorbe[i].open-minVal)*(yL-yT)/(maxVal-minVal));
                int y2 = yL-((gorbe[i].close-minVal)*(yL-yT)/(maxVal-minVal));
                if (gorbe[i].open<gorbe[i].close)
                    lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,0,255,0,255);
                else lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,255,0,0,255);
                /// függőleges válaszfalak napi bontáshoz
                if (i%60==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,30);
                else if (i%30==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,60);
            }
            else { /// egy negyedévről van szó
                int y1 = yL-((gorbe[i].open-minVal)*(yL-yT)/(maxVal-minVal));
                int y2 = yL-((gorbe[i].close-minVal)*(yL-yT)/(maxVal-minVal));
                if (gorbe[i].open<gorbe[i].close)
                    lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,0,255,0,255);
                else lineRGBA(renderer,wa+5+i,wb+y1,wa+5+i,wb+y2,255,0,0,255);
                /// függőleges válaszfalak napi bontáshoz
                if (i%60==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,30);
                else if (i%30==0) lineRGBA(renderer,wa+5+i,wb+yT,wa+5+i,wb+yL,255,255,255,60);
            }

        }

    }

    int state = 0; /// 0 semmi, 1 napi év, 2 hónap, 3 nap, 4 negyedéves mező

    bool inClick(int bx, int by){
        if (napiB.inClick(bx,by)){
            type=0;
            napiB.selected=true; negyedeviB.selected=false;
            setLastNap();
            gorbeFrissitese();
        }
        else if (negyedeviB.inClick(bx,by)){
            type=1;
            napiB.selected=false; negyedeviB.selected=true;
            setLastNegyed();
            gorbeFrissitese();
        }
        else if (elozohozB.inClick(bx,by)){
            elozohozB.selected=!elozohozB.selected;
            gorbeFrissitese();
        }
        else if (szazalekB.inClick(bx,by)){
            szazalekB.selected=!szazalekB.selected;
            gorbeFrissitese();
        }
        else if (datumOKB.inClick(bx,by)){
            gorbeFrissitese();
        }
        else if (type==0 && evB.inClick(bx,by)){
            state=1;
        }
        else if (type==0 && honapB.inClick(bx,by)){
            state=2;
        }
        else if (type==0 && napB.inClick(bx,by)){
            state=3;
        }
        else if (type==1 && negyedevB.inClick(bx,by)){
            state=4;
        }
        else if (elozoDatumB.inClick(bx,by)){
            state=0;
            if (type==0) setBeforeNap();
            if (type==1) setBeforeNegyed();
            gorbeFrissitese();
        }
        else if (kovetkezoDatumB.inClick(bx,by)){
            state=0;
            if (type==0) setNextNap();
            if (type==1) setNextNegyed();
            gorbeFrissitese();
        }
        else {
            state=0;
        }
        return false;
    }

    bool inInput(char c){
        if (state==1) {
            if (isalnum(c) && !isalpha(c) && evB.str.size()<4){
                evB.str+=c;
            }
        }
        if (state==2) {
            if (isalnum(c) && !isalpha(c) && evB.str.size()<2){
                honapB.str+=c;
            }
        }
        if (state==3) {
            if (isalnum(c) && !isalpha(c) && evB.str.size()<2){
                napB.str+=c;
            }
        }
        if (state==4) {
            if ((isalnum(c) && !isalpha(c) || c==' ')  && evB.str.size()<10){
                negyedevB.str+=c;
            }
        }
        return true;
    }

    bool inDeleteButton(){
        if (state==1) {
            if (evB.str.size()>0){
                evB.str.pop_back();
            }
        }
        if (state==2) {
            if (honapB.str.size()>0){
                honapB.str.pop_back();
            }
        }
        if (state==3) {
            if (napB.str.size()>0){
                napB.str.pop_back();
            }
        }
        if (state==4) {
            if (negyedevB.str.size()>0){
                negyedevB.str.pop_back();
            }
        }
        return true;
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

    bool inClickGorbek(int bx ,int by){
        for (int i=0; i<gorbek.size(); i++)
            if (gorbek[i].inClick(bx,by))
                return true;
        return false;
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
        rectangleRGBA(renderer, wa, wb, wa+75+gorbek.size()*410, wb+200,0,0,0,255);
        UjElemB.draw(renderer,wa+5,wb+30);
        ElemzesB.draw(renderer,wa+5,wb+60);
        if (!isLocked(mfs))
            stringRGBA(renderer,wa+5,wb+5,stock.name.c_str(),0,0,0,255);
        for (int i=0; i<gorbek.size(); i++){
            gorbek[i].draw(renderer,wa+80+410*i,wb+10);
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
        else if (inClickGorbek(bx,by)){
            return true;
        }
        return false;
    }

    bool inInput(char c){
        for (int i=0; i<gorbek.size(); i++) {
            gorbek[i].inInput(c);
        }
        return true;
    }

    bool inDeleteButton(){
        for (int i=0; i<gorbek.size(); i++) {
            gorbek[i].inDeleteButton();
        }
        return true;
    }
};

#endif // BUTTON_H_INCLUDED
