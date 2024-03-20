#include "common.h"


#include <list>
#include <utility>
#include <mutex>
#include <memory>
using namespace std;


#include <random>
#include <algorithm>

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "cimke.h"
#include "cimkeSrc/OsszesCimke.h"

#include "Menu.h"


//mutex draw;

int dataIdx = 0;
int MX, MY;

bool kirajzol = false;
int colorcnt=0;

void draw(SDL_Renderer &renderer){
    //SDL_Rend
    cout<<SDL_SetRenderDrawColor(&renderer,colorcnt%255,0,0,255);
    cout<<SDL_RenderClear(&renderer);
    colorcnt++;

    //boxRGBA(&renderer,i,x-1,i+1,x+1,255,255*(v-locMin)/(locMax-locMin),255,255);
    //stringRGBA(&renderer,20,MAGAS-37,ss2.str().c_str(),200,200,200,255);

    cout<<"alma"<<endl;
    stringRGBA(&renderer,20,20,"almafa",126,250,20,255);
    SDL_RenderPresent(&renderer);
}

void megjelenites(Menu *actualMenu){

    cout<<actualMenu<< " "<<&actualMenu <<endl;
    while (true){
        clock_t t1=clock();
        clock_t delay=60;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            //cout<<"alma"<<clock()<<endl;
            //if (kirajzol){
            actualMenu->draw();
                //draw(renderer);
                //cout<<"a";
                //Draw_Mezok(renderer, mezok, window);
            //}
            t1=clock();
        } else {
            Sleep(1);
        }
    }
}

void esemenyKezel(Menu *actualMenu){
    Sleep(10);
    cout<<actualMenu<< " "<<&actualMenu <<endl;
    while (true){
        SDL_Event ev;
        SDL_PollEvent(&ev);
        actualMenu->inputHandle();
        Sleep(1);
    }
}


void konzolKezel(){
    while (true){
        string inp;
        cin>>inp;
        cout<<inp<<endl;
    }
}

bool getValueFromIdopont(float &ret, int o, int p, set<Arfolyam> &ertekek){
    Idopont ido(o,p);
    Arfolyam af(ido.ora,ido.perc);
    set<Arfolyam>::iterator e = ertekek.find(af);
    if (e!=ertekek.end()){
        ret=e->open;
        if (ret==-1 || !ret*2>ret)
            return false;
    } else {
        return false;
    }
    return true;
}

void iterFV(int iC,vector<array<array<bool,870>,870>>& kombinaciok ,vector<vector<array<array<float,10>,870>>>& kombinaciokProfitja ,vector<bool>& kombok, float startV, float endV, int thCnt, int nap){
    int ic1=iC;
    nap-=1; nap*=2;
    for (int ic1 = iC;ic1<iC+thCnt && ic1<435;ic1++){
    ///cout<<ic1<<endl;
    for (int k1=0; k1<2; k1++){
        for (int k2=0;k2<2; k2++){
            for (int k3=0;k3<2;k3++){
//    ths[ic1] = thread(iterFV,ic1,ref(kombinaciok),ref(kombinaciokProfitja),ref(kombok),startV,endV);
    for (int ic2=ic1+1; ic2<435; ic2++){
        for (int ic3=ic2+1; ic3<435; ic3++){
                        int c1=ic1, c2=ic2 ,c3=ic3;
                        if (k1==1) c1+=435;
                        if (k2==1) c2+=435;
                        if (k3==1) c3+=435;
                        kombinaciok[c1][c2][c3]=kombok[c1]&&kombok[c2]&&kombok[c3];

                        if (kombinaciok[c1][c2][c3]){
                            kombinaciokProfitja[c1][c2][c3][nap+0] *= startV/endV*0.995f;
                        } else {
                            kombinaciokProfitja[c1][c2][c3][nap+1] *= endV/startV*0.995f;
                        }
                        //cout<<"a"<<endl;
                    }
                }
            }
        }
    }
    }
    return;
}

void main2( SDL_Window &window, SDL_Renderer &renderer){
    srand(time(NULL));
    SDL_Event ev;

     AppConfig& config = AppConfig::getInstance();
     config.loadConfig();
     cout<<config.getRootDirectory()<<" "<<config.getMaxThreads()<<endl;

    /// adatokat kell beolvasnom
    clock_t t = clock();
    Stock stock1,stock2,stock3,stock4,stock5,stock;

    ///loadStock("TSLA",stock1);
    /*
    cout<<"z"<<endl;
    loadStock2("TSLA",stock2);
    cout<<"zz"<<endl;
    loadStock3("TSLA",stock3);
    cout<<"zzz"<<endl;
    loadStock2("MCD",stock4);
    cout<<"zzzz"<<endl;
    loadStock2("WMT",stock5);
    */

    cout<<"a1: "<<(clock()-t)<<endl;
    t=clock();

    SDL_Pack sdlp(&window,&renderer,&ev);
    Menu *actualMenu;
    MenuK menuk(sdlp,&actualMenu);


    /*
    Stock stock11,stock22,stock33,stock44,stock55;
    thread th1(loadStock,"NVDA",ref(stock11));
    thread th2(loadStock,"AMD",ref(stock22));
    thread th3(loadStock,"NFLX",ref(stock33));
    thread th4(loadStock,"AA",ref(stock44));
    thread th5(loadStock,"MCD",ref(stock55));
    th1.join();th2.join();th3.join();th4.join();th5.join();
    */
    /// manuális teszt
    if (true){
    vector<string> reszvenyekNeve = csoportReszvenyei("osszes"); //osszesReszveny();
    reszvenyekNeve = osszesReszveny();
    int thCnt = 16;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<Stock> stocks; stocks.resize(thCnt);
    long long osszesPelda = 0, joPelda = 0;
    int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
    float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;
    bool m;
    vector<ofstream> fajlok(thCnt);
    ofstream fajl("fofajl.txt");
    for (size_t i=0;i<0;i++){
        string fajlnev = "alma" + i;
        fajlnev = fajlnev + ".txt";
        fajlok[i].open(fajlnev);
    }
    cout<<reszvenyekNeve.size()<<endl;
    Datum utolsoNap(2024,1,29);
    Datum elsoNap(2014,1,8);
    for (size_t i=1000; i<reszvenyekNeve.size();){
        for (int j=0; j<thCnt; j++){

            szalak[j] = thread(loadStock,reszvenyekNeve[i],ref(stocks[j]),ref(m));
            i++;
            cout<<i<<endl;
            if (i>=reszvenyekNeve.size()) break;
        }


        for (int j=0; j<thCnt; j++){
            if (szalak[j].joinable())
                szalak[j].join();
            Nap nap; Datum datum = elsoNap;
            stocks[j].getNap(nap,datum);
            while(nap.datum<utolsoNap){
                Idopont ido = nap.idoNyitas;
                set<Arfolyam> ertekek = nap.percek;
                bool ok = true;
                float adatok[30];
                for (int k=0; k<30; k++){
                    ok = ok && getValueFromIdopont(adatok[k],ido.ora,ido.perc,ertekek);
                    ido.kovetkezoPerc();
                }
                float zar;
                ok = ok && getValueFromIdopont(zar,nap.idoZaras.ora,nap.idoZaras.perc,ertekek);
                /// ALAPADATOK

                float erdekesPercek[5];
                ok = ok && getValueFromIdopont(erdekesPercek[0],10,0,ertekek);
                ok = ok && getValueFromIdopont(erdekesPercek[1],10,2,ertekek);
                ok = ok && getValueFromIdopont(erdekesPercek[2],10,5,ertekek);
                ok = ok && getValueFromIdopont(erdekesPercek[3],15,56,ertekek);
                ok = ok && getValueFromIdopont(erdekesPercek[4],15,59,ertekek);

                bool felmegy = adatok[29]<zar;
                float maxiPerMini = 999999;
                if (felmegy) maxiPerMini = 0;
                Idopont vege = nap.idoZaras; vege.kovetkezoPerc();
                Idopont keresettIdo;
                ido = nap.idoNyitas;
                for (int k=0; ido<vege; k++){
                    float ret  = 0;
                    ok = ok && getValueFromIdopont(ret,ido.ora,ido.perc,ertekek);
                    if (!ok) break;
                    if (felmegy) {
                        if (ret>maxiPerMini){
                            maxiPerMini=ret;
                            keresettIdo=ido;
                        }
                    } else {
                        if (ret<maxiPerMini){
                            maxiPerMini=ret;
                            keresettIdo=ido;
                        }
                    }
                    ido.kovetkezoPerc();
                }

                if (ok){

                    for (int k=0; k<30; k++){
                        fajl<<adatok[k]<<" ";
                    }
                    fajl<<zar<<" ";
                    fajl<<stocks[j].name<<" ";
                    fajl<<nap.datum.toString()[0]<<" "<<nap.datum.toString()[1]<<" "<<nap.datum.toString()[2]<<" ";
                    for (int k=0; k<5; k++){
                        fajl<<erdekesPercek[k]<<" ";
                    }
                    fajl<<maxiPerMini<<" "<<keresettIdo.ora<<" "<<keresettIdo.perc<<endl;
                }
                ///cout<<nap.datum.year<<" "<<nap.datum.month<<" "<<nap.datum.day<<endl;
                //cout<<nap.kovetkezoNap.toString()[0]<<" "<<nap.kovetkezoNap.toString()[1]<<" "<<nap.kovetkezoNap.toString()[2]<<endl;
                if (nap.kovetkezoNap.year==-1) break;
                stocks[j].getNap(nap,nap.kovetkezoNap);

            }
        }
    }
    fajl.close();
    for (size_t i=0;i<0;i++){
        fajlok[i].close();
    }
    }

    /// Egy speciális eset
    if (true){
    vector<string> reszvenyekNeve = csoportReszvenyei("osszes"); //osszesReszveny();
    cout<<reszvenyekNeve.size()<<endl;
    //reszvenyekNeve = osszesReszveny();
    reszvenyekNeve.clear(); reszvenyekNeve.push_back("NVDA");
    int thCnt = 1;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<Stock> stocks; stocks.resize(thCnt);
    bool m;
    long long osszesPelda = 0, joPelda = 0;
    int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
    float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;


    cout<<"alma"<<endl;
    vector<vector<array<array<float,10>,870>>> kombinaciokProfitja;
    cout<<"alma"<<endl;
    if (true){
        array<float,10> basic = {1,1,1,1,1,1,1,1,1,1};
        array<array<float,10>,870> vec3; for (int ic=0; ic<870; ic++) vec3[ic]=basic;
        vector<array<array<float,10>,870>> vec4; for (int ic=0; ic<870; ic++) vec4.push_back(vec3);
        for (int ic=0; ic<870; ic++) kombinaciokProfitja.push_back(vec4);
    }

    vector<array<array<bool,870>,870>> kombinaciok;
    if (true){
        bool basic = false;
        array<bool,870> vec3; for (int ic=0; ic<870; ic++) vec3[ic]=basic;
        array<array<bool,870>,870> vec4; for (int ic=0; ic<870; ic++) vec4[ic]=vec3;
        for (int ic=0; ic<870; ic++) kombinaciok.push_back(vec4);
    }
    cout<<"alma"<<endl;


    float zL = 0, zIL = 0, oE = 0, fJ = 0, pF = 1.0f;
    float zL2 = 0, zIL2 = 0, oE2 = 0, fJ2 = 0, pF2 = 1.0f;
    for (size_t i=0; i<reszvenyekNeve.size() && i<1; i++){
        int savedI = i;
        for (int j=0; j<thCnt; j++){
            szalak[j] = thread(loadStock,reszvenyekNeve[i],ref(stocks[j]),ref(m));
            i++;
            //cout<<i<<endl;
            if (i>=reszvenyekNeve.size()) break;
            cout<<i<<endl;
        }

        for (int j=0; j<thCnt; j++){
            if (savedI+j>=reszvenyekNeve.size()) continue;
            clock_t t1 = clock();
            if (szalak[j].joinable())
                szalak[j].join();
            if (stocks[j].negyedevek.size()<3) continue;
            Nap kezdonap(2015,1,5);
            //Nap kezdonap(2023,1,9);
            Nap ntemp(2023,10,18);
            if (stocks[j].mindenNap.find(ntemp) == stocks[j].mindenNap.end()) {
                Nap n2temp = *(stocks[j].mindenNap.rbegin());
                if (n2temp.datum.year==2023 && n2temp.datum.month==10 && n2temp.datum.day==13){}
                else continue;
                cout<<stocks[j].name<<" "<<n2temp.datum.year<<" "<<n2temp.datum.month<<" "<<n2temp.datum.day<<endl;

            }
            set<Nap>::iterator reszvenyNapjai = stocks[j].mindenNap.find(kezdonap);
            if (reszvenyNapjai==stocks[j].mindenNap.end()) continue;

            float felfeleJobban = 0, osszesEset = 0, zarasLejebb = 0, zarasIsLejebb = 0;
            float profit = 1.0f, minNagyafter = 0 ,minNK = 0, startKisebb = 0;
            int nCnt = 0;
            for (;reszvenyNapjai!=stocks[j].mindenNap.end();){
                ///cout<<j<<" "<<nCnt<<endl;
                nCnt++;
                //cout<<reszvenyNapjai->datum.day<<endl;;
                Arfolyam perc(9,30);
                Arfolyam percZ(15,58);
                set<Arfolyam>::iterator kperc =  reszvenyNapjai->percek.find(perc);
                set<Arfolyam>::iterator kpercZ =  reszvenyNapjai->percek.find(percZ);
                if (reszvenyNapjai->percek.end()==kperc || reszvenyNapjai->percek.end()==kpercZ) continue;
                float ertekek[30], startV;
                for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kperc && cntPerc<30; cntPerc++){
                    ertekek[cntPerc]=kperc->open;
                    startV=kperc->close;
                    kperc++;
                }
                Arfolyam percK(10,0);
                float endV = 0.0f;
                set<Arfolyam>::iterator kpercK =  reszvenyNapjai->percek.find(percK);
                if (reszvenyNapjai->percek.end()==kpercK) continue;
                for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kpercK && cntPerc<358; cntPerc++){
                    endV = kpercK->close;
                    kpercK++;
                }


                vector<bool> kombok(870);
                int icC = 0;
                for (int ic1=0; ic1<30; ic1++){
                    for (int ic2=ic1+1; ic2<30; ic2++){
                        if (ertekek[ic1]<ertekek[ic2]) {kombok[icC]=true; kombok[icC+435]=false; }
                        else {kombok[icC]=false; kombok[icC+435]=true; }
                        icC++;
                    }
                }
                //cout<<icC<<endl;

                vector<thread> ths(435);
                for (int ic1=0; ic1<435; ic1+=thCnt){
                    //cout<<ic1<<endl;
                    ths[ic1] = thread(iterFV,ic1,ref(kombinaciok),ref(kombinaciokProfitja),ref(kombok),startV,endV,thCnt,reszvenyNapjai->hetMelyikNapja);

                    actualMenu->inputHandle();
                ///sleep(1);
                    //cout<<ic1<<endl;
                    /**
                    for (int k1=0; k1<2; k1++){
                        for (int k2=0;k2<2; k2++){
                            for (int k3=0;k3<2;k3++){
                    for (int ic2=ic1+1; ic2<435; ic2++){
                        for (int ic3=ic2+1; ic3<435; ic3++){
                                        int c1=ic1, c2=ic2 ,c3=ic3;
                                        if (k1==1) c1+=435;
                                        if (k2==1) c2+=435;
                                        if (k3==1) c3+=435;
                                        kombinaciok[c1][c2][c3]=kombok[c1]&&kombok[c2]&&kombok[c3];
                                        if (kombinaciok[c1][c2][c3]){
                                            kombinaciokProfitja[c1][c2][c3][0] *= startV/endV*0.995f;
                                        } else {
                                            kombinaciokProfitja[c1][c2][c3][1] *= endV/startV*0.995f;
                                        }
                                        //cout<<"a"<<endl;
                                    }
                                }
                            }
                        }
                    }
                    //*/
                }
                for (int ic1=0; ic1<435; ic1+=thCnt){
                    ths[ic1].join();
                }


                reszvenyNapjai++;

            }
            //*/
            //n0--;
            /*
            for (;n1!=stocks[j].negyedevek.end();++n0,++n1){

                Nap dv((*n0).negyedevVege);
                set<Nap>::iterator it = stocks[j].mindenNap.find(dv);
                if (it==stocks[j].mindenNap.end()) continue;
                dv = *it;

                Nap d0, d1, d2;
                d2=*(--it);
                d1=*(--it);
                d0=*(--it);

                /// kisebb szűrési feltétel
                if (d0.datum<Datum(2023,1,1)){
                    //continue;
                }
                else {
                    //break;
                }

                float biztossag = 0.8f;
                float e1=0, e2=0, e3=0, e4=0, e5=0, e6=0;
                /// feltételek

                stocks[j].getNyit(e1,d0,true);
                stocks[j].getMin(e2,d0,true);
                stocks[j].getMax(e3,d0,true);
                stocks[j].getZar(e4,d0,true);
                if ( (e4-e2)/(e3-e2) < 0.35f ) continue; /// Bullis Pin alja a 80%-nál van
                if ( e1<e4 ) continue;  /// aznapi elmozdulás pozitív volt

                z1++;

                stocks[j].getNyit(e5,d1,true);
                if (e1>e5) continue;
                e1=e5;
                stocks[j].getMin(e5,d1,true);
                if (e2<e5) continue;
                e2=e5;
                stocks[j].getMax(e3,d1,true);
                stocks[j].getZar(e4,d1,true);
                if (e1==0 || e2==0 || e3==0 || e4==0) {//cout<<"BAJ 0"<<endl;
                    cout<<e1<<" "<<e2<<" "<<e3<<" "<<e4<<endl;
                    cout<<stocks[j].name<<" "<<d0.datum.year<<" "<<d0.datum.month<<" "<<d0.datum.day<<endl;
                    cout<<d0.minimum<<" "<<d0.maximum<<" "<<d0.nyitas<<" "<<d0.zaras<<endl;
                continue;}


                z2++;

                if ( (e4-e2)/(e3-e2) > 0.1f ) continue; /// Bullis Pin alja a 80%-nál van
                if ( (e1-e2)/(e3-e2) < 0.6f ) continue; /// Bullis Pin alja a 80%-nál van
                if ( e1<e4 ) continue;  /// aznapi elmozdulás pozitív volt

                z3++;

                stocks[j].getNyit(e1,d2,true);
                if (e1<e4) continue;
                stocks[j].getMin(e5,d2,true);
                if (e2<e5) continue; e2=e5;
                stocks[j].getMax(e3,d2,true);
                stocks[j].getZar(e4,d2,true);

                z4++;

                if ( e4/e1 > 1.004f) continue;
                if ( (e1-e2)/(e3-e2) > 0.3f ) continue; /// Bullis Pin alja a 80%-nál van

                z5++;

                if ( (e1-e2)/(e3-e2) < 0.2f ) continue; /// Bullis Pin alja a 80%-nál van
                if ( e4<e1 ) continue;  /// aznapi elmozdulás pozitív volt


                /// Tényleges találat
                osszesPelda++;


                //stocks[j].getZar(e5,d1,true); stocks[j].getZar(e6,d1,true); if (e5>e4) z1++;
                //stocks[j].getZar(e5,d2,true); stocks[j].getZar(e6,d2,true); if (e5>e4) z2++;
                //stocks[j].getZar(e5,d3,true); stocks[j].getZar(e6,d3,true); if (e5>e4) z3++;
                //stocks[j].getZar(e5,d4,true); stocks[j].getZar(e6,d4,true); if (e5>e4) z4++;
                //stocks[j].getZar(e5,d5,true); stocks[j].getZar(e6,d5,true); if (e5>e4) z5++;
                //stocks[j].getZar(e5,d6,true); stocks[j].getZar(e6,d6,true); if (e5>e4) z6++;

                cout<<"alma "<<stocks[j].name<<" "<<dv.datum.year<<" "<<dv.datum.month<<" "<<dv.datum.day<<endl;
                /// Helyes elmozdulás
                if (dv.nyitas<e4) continue;
                joPelda++;
                //cout<<"osszes, jo: "<<osszesPelda<<" "<<joPelda<<" "<<(float)joPelda/osszesPelda<<endl;

            }
            */
            //cout<<i+j<<endl;
            cout<<j<<", T: "<<clock()-t1<<endl;
        }

        sleep(1);
        //**
        cout<<"stabil: "<<fJ/oE*100.f<<" "<<zL/fJ*100.f<<" "<<zIL/zL*100.f<<" "<<zIL<<" / "<<zL<<" / "<<fJ<<" / "<<oE<<endl;
        cout<<"minden: "<<fJ2/oE2*100.f<<" "<<zL2/fJ2*100.f<<" "<<zIL2/zL2*100.f<<" "<<zIL2<<" / "<<zL2<<" / "<<fJ2<<" / "<<oE2<<endl;
        cout<<"stabil: "<<pF/z1<<", "<<f1/fJ<<" "<<f2/f3<<endl;
        cout<<"minden: "<<pF2/z2<<", "<<f4/fJ2<<" "<<f5/f6<<endl;

    }

    ofstream of("kimenet.txt");

        for(int k1=0;k1<870;k1++){
            cout<<"k1: "<<k1<<endl;
            for(int k2=0;k2<870;k2++){
                for(int k3=0;k3<870;k3++){
                    for(int k4=0;k4<10;k4++){
                    of<<kombinaciokProfitja[k1][k2][k3][k4]<<" ";
                }
            }
            of<<endl;
        }
        of<<endl<<endl;
    }

        cout<<"almak"<<endl;
    of.close();

    cout<<z1<<" "<<z2<<" "<<z3<<" "<<z4<<" "<<z5<<" "<<z6<<endl;
    if (osszesPelda>0)cout<<z1*100/osszesPelda<<" "<<z2*100/osszesPelda<<" "<<z3*100/osszesPelda<<" "<<z4*100/osszesPelda<<" "<<z5*100/osszesPelda<<" "<<z6*100/osszesPelda<<endl;
    cout<<"osszes, jo: "<<osszesPelda<<" "<<joPelda<<" "<<(float)joPelda/osszesPelda<<endl;
    }
    //*/



    //stock.adatokBetoltese("NVDA");
    cout<<"a2: "<<(clock()-t)<<endl;
    t=clock();
    stock.adatokFeldolgozasa();
    cout<<"a3: "<<(clock()-t)<<endl;
    t=clock();
    //stock.adatokKiirasaFajlba("NVDA");
    cout<<"a4: "<<(clock()-t)<<endl;
    t=clock();


    /*
    Stock ss2;
    cout<<"a1: "<<(clock()-t)<<endl;
    t=clock();
    ss2.adatokBetoltese2Teszt("AMD");
    cout<<"a2: "<<(clock()-t)<<endl;
    t=clock();
    //ss2.adatokKiirasaFajlba("AMD2");
    cout<<"a3: "<<(clock()-t)<<endl;
    t=clock();
    */
    //cout<<_msize(stock)<<endl;






    /*
    vector<string> fajlok = getFiles(gyoker+"\\stocks\\AAPL\\months");
    //for (int i=0; i<fajlok.size(); i++) cout<<fajlok[i]<<endl;

    string fileP = gyoker+"\\stocks\\AAPL\\months\\"+fajlok[0];
    ifstream file(fileP);
    while (false && getline(file,fileP)) {
        stringstream ss; ss<<fileP;
        char c;
        int year=0, month=0, day=0, hour=0, minute=0;
        ss>>year>>c>>month>>c>>day>>hour>>c>>minute;
        ss>>c>>c>>c>>c;
        float open, maxi, mini, close, vol;
        ss>>open>>c>>maxi>>c>>mini>>c>>close>>c>>vol;
        cout<<year<<" "<<month<<" "<<day<<" "<<hour<<" "<<minute<<" "<<open<<" "<<maxi<<" "<<mini<<" "<<close<<" "<<vol<<endl;
            //cout<<fileP<<" alma"<<endl;

    }*/

    /*
    LokMinMax m;
    Cimke c;
    Idosor ids;
    m.azIdosorIlyen(ids);
    c.azIdosorIlyen(ids);

    std::vector<Cimke*> animals;

    animals.push_back(&m);
    animals.push_back(&c);
    */


    //cout<<m.type<<c.type<<endl;



    //cout<<actualMenu<< " "<<&actualMenu <<endl;

    /// megjelenítés még kicsit hibás, nem tudom külön szálon futtatni
    //thread frame(megjelenites,ref(actualMenu));   /// megjelenítás
    //thread esemeny(esemenyKezel,ref(actualMenu)); /// SDL input feldolgozás
    //thread konzol(konzolKezel);                             /// konzol input feldolgozás
    int frameCnt = 0;
    t = clock();
    while(true){
        frameCnt++;
        if (t+1000<=clock()){
            //cout<<"FPS: "<<frameCnt<<endl;
            frameCnt=0;
            t=clock();
        }
        //SDL_PollEvent(&ev);
        //cout<<"a";
        actualMenu->inputHandle();
        //cout<<"b";
        actualMenu->process();
        //cout<<"c";
        actualMenu->draw();
        //SDL_PollEvent(&ev);                                 /// Ez kell az input feldolgozáshoz külön szálon
        //if (!kirajzol){}
          //  draw(renderer);
        Sleep(1);
        //ev.window.event
        //cout<<ev.window.event<<endl;
        if (ev.window.event == SDL_WINDOWEVENT_RESIZED){
            //cout<<"resized"<<endl;
            int x, y;
            SDL_GetWindowSize(&window,&x,&y);
            //cout<<x<<" "<<y<<endl;
            //frame = thread(megjelenites,ref(renderer),ref(window));
        }
    }
}
