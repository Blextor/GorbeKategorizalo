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

void iterFV(int iC,vector<array<array<bool,435>,435>>& kombinaciok ,vector<vector<array<array<float,80>,435>>>& kombinaciokProfitja ,vector<bool>& kombok, float startV, float endV, int thCnt, int nap){
    int ic1=iC;
    nap-=1; nap*=16;
    for (int ic1 = iC;ic1<iC+thCnt && ic1<435;ic1++){
    //cout<<ic1<<endl;
    //for (int k1=0; k1<2; k1++){
      //  for (int k2=0;k2<2; k2++){
        //    for (int k3=0;k3<2;k3++){
//    ths[ic1] = thread(iterFV,ic1,ref(kombinaciok),ref(kombinaciokProfitja),ref(kombok),startV,endV);
        for (int ic2=ic1+1; ic2<435; ic2++){
            for (int ic3=ic2+1; ic3<435; ic3++){
                            int c1=ic1, c2=ic2 ,c3=ic3;
                            //if (k1==1) c1+=435;
                            //if (k2==1) c2+=435;
                            //if (k3==1) c3+=435;
                            if (kombok[c1]&&kombok[c2]&&kombok[c3]){
                                kombinaciokProfitja[c1][c2][c3][nap+0] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+1] *= endV/startV*0.995f;
                            } else {
                                kombinaciokProfitja[c1][c2][c3][nap+2] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+3] *= endV/startV*0.995f;
                            }
                            if (!kombok[c1]&&kombok[c2]&&kombok[c3]){
                                kombinaciokProfitja[c1][c2][c3][nap+4] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+5] *= endV/startV*0.995f;
                            } else {
                                kombinaciokProfitja[c1][c2][c3][nap+6] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+7] *= endV/startV*0.995f;
                            }
                            if (kombok[c1]&&!kombok[c2]&&kombok[c3]){
                                kombinaciokProfitja[c1][c2][c3][nap+8] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+9] *= endV/startV*0.995f;
                            } else {
                                kombinaciokProfitja[c1][c2][c3][nap+10] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+11] *= endV/startV*0.995f;
                            }
                            if (kombok[c1]&&kombok[c2]&&!kombok[c3]){
                                kombinaciokProfitja[c1][c2][c3][nap+12] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+13] *= endV/startV*0.995f;
                            } else {
                                kombinaciokProfitja[c1][c2][c3][nap+14] *= startV/endV*0.995f;
                                kombinaciokProfitja[c1][c2][c3][nap+15] *= endV/startV*0.995f;
                            }
                            //cout<<"a"<<endl;
                        }
                    }
                //}
            //}
        //}
    }
    return;
}

/*
struct EarningsPlus{
    string stockName = "";
    Datum datumElso, datumUtolso;
    bool post = true;
    string hanyQ;
    float eps, est_eps;
};

vector<string> loadEarnings(vector<EarningsPlus> &earningsV){
    cout << "Hello world!" << endl;
    ifstream inputFile("earnings_modified2.csv");
    string line;
    getline(inputFile,line);
    cout<<line<<endl;
    char delimiter = ',';
    char delimiter2 = '/';
    EarningsPlus before;
    earningsV.clear();
    vector<string> ret;
    while (getline(inputFile,line)){
        std::vector<std::string> result;// = splitString(line, delimiter);
        EarningsPlus earnings;
        earnings.stockName=result[0];
        stringstream ss2(result[1]); /// év
        stringstream ss3(result[2]); /// hónap
        stringstream ss4(result[3]); /// nap
        Datum datum; ss2>>datum.year; ss3>>datum.month; ss4>>datum.day;
        earnings.datumUtolso=datum;
        earnings.hanyQ = result[4];
        stringstream ss5(result[5]); /// est_eps
        stringstream ss6(result[6]); /// eps
        ss5>>earnings.est_eps;
        ss6>>earnings.eps;
        earnings.post=(result[7]=="post");
        if (before.stockName==earnings.stockName){
            earnings.datumElso=
        }
        before=earnings;
    }
}
*/

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
    if (false){
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


    /// Adat lekérdezés
    if (false){
        vector<string> reszvenyekNeve = {"AA"};//csoportReszvenyei("estere");//{"NVDA"};//csoportReszvenyei("estere");
        reszvenyekNeve = csoportReszvenyei("osszesUj.txt");

        for (int i=0; i<reszvenyekNeve.size();){
            for (;i<reszvenyekNeve.size(); i++)
                cout<<reszvenyekNeve[i]<<" ";
            cout<<endl;
        }
        //reszvenyekNeve.clear(); reszvenyekNeve.push_back("NFLX"); reszvenyekNeve.push_back("ADBE"); reszvenyekNeve.push_back("NVDA");
        cout<<reszvenyekNeve.size()<<endl;
        int thCnt = 32;
        vector<thread> szalak; szalak.resize(thCnt);
        vector<Stock> stocks; stocks.resize(thCnt);
        bool m;
        long long osszesPelda = 0, joPelda = 0;
        int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
        float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;


        ofstream kimenetiFajl("negyedOssz2.txt");
        kimenetiFajl<<"name year month day elozoZaras elsoNyitasErtek elsoZarasErtek utolsoNyitasErtek utolsoZarasErtek ";
        kimenetiFajl<<"kiugras napVegiKiugras elsoNapDiff elsoNapIngadozas elsoNapAlsoIngadozas elsoNapFelsoIngadozas ";
        kimenetiFajl<<"negyedDiff negyedDiffElsoNap negyedIngadozas negyedAlsoIngadozas negyedFelsoIngadozas ";
        kimenetiFajl<<"napCnt ";
        kimenetiFajl<<"negyedGlobMinHely negyedGlobMaxHely negyedGlobMinErtek negyedGlobMaxErtek ";
        kimenetiFajl<<"negyedLokMinCnt15 negyedLokMaxCnt15";
        kimenetiFajl<<endl;
        for (size_t i=0; i<reszvenyekNeve.size();){
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
                cout<<stocks[j].name<<endl;

                Negyed elozo; elozo.income=-3;
                for (const Negyed &negyed: stocks[j].negyedevek){
                    Negyed mostani=negyed;
                    if (elozo.income==-3){
                        elozo=negyed;
                        continue;
                    }
                    set<Nap>::iterator negyedElsoNapja = stocks[j].mindenNap.find(elozo.korrigaltTenylegesJelentes);
                    //if (negyedElsoNapja==stocks[j].mindenNap.end())
                      //  cout<<elozo.korrigaltTenylegesJelentes.year<<" "<<elozo.korrigaltTenylegesJelentes.month<<" "<<elozo.korrigaltTenylegesJelentes.day<< " END"<<endl;
                    set<Nap>::iterator negyedMegelozoNapja = negyedElsoNapja; negyedMegelozoNapja--;
                    set<Nap>::iterator negyedUtolsoUtaniNapja = stocks[j].mindenNap.find(negyed.korrigaltTenylegesJelentes);
                    set<Nap>::iterator negyedUtolsoNapja = negyedUtolsoUtaniNapja; negyedUtolsoNapja--;
                    //Nap nap; nap.elozohozKiugras

                    /// name year month day
                    kimenetiFajl<<stocks[j].name<<" "<<negyed.idoszakVege.year<<" "<<negyed.idoszakVege.month<<" "<<negyed.idoszakVege.day<<" ";

                    /// elozoZaras elsoNyitasErtek elsoZarasErtek utolsoNyitasErtek utolsoZarasErtek
                    kimenetiFajl<<negyedMegelozoNapja->zaras<<" "<<negyedElsoNapja->nyitas<<" "<<negyedElsoNapja->zaras<<" ";
                    kimenetiFajl<<negyedUtolsoNapja->nyitas<<" "<<negyedUtolsoNapja->zaras<<" ";

                    /// kiugras napVegiKiugras elsoNapDiff elsoNapIngadozas elsoNapAlsoIngadozas elsoNapFelsoIngadozas
                    float elsoNapLegkisebbNyitoVagyZaro = min(negyedElsoNapja->nyitas,negyedElsoNapja->zaras);
                    float elsoNapLegnagyobbNyitoVagyZaro = max(negyedElsoNapja->nyitas,negyedElsoNapja->zaras);
                    float elsoNapIngadozasAlso = elsoNapLegkisebbNyitoVagyZaro/negyedElsoNapja->minimum;
                    float elsoNapIngadozasFelso = negyedElsoNapja->maximum/elsoNapLegnagyobbNyitoVagyZaro;
                    float elsoNapIngadozas = negyedElsoNapja->maximum/negyedElsoNapja->minimum;
                    kimenetiFajl<<negyedElsoNapja->nyitas/negyedMegelozoNapja->zaras<<" "<<negyedElsoNapja->zaras/negyedMegelozoNapja->zaras<<" "; /// kiugras napVegiKiugras
                    kimenetiFajl<<negyedElsoNapja->zaras/negyedElsoNapja->nyitas<<" "<<elsoNapIngadozas<<" "<<elsoNapIngadozasAlso<<" "<<elsoNapIngadozasFelso<<" "; /// elsoNapDiff elsoNapIngadozas elsoNapAlsoIngadozas elsoNapFelsoIngadozas

                    /// negyedDiff negyedDiff_ElsoNap negyedIngadozas negyedAlsoIngadozas negyedFelsoIngadozas
                    float negyedDiff = negyedUtolsoNapja->zaras/negyedElsoNapja->nyitas;
                    float negyedDiff_ElsoNap=negyedUtolsoNapja->zaras/negyedElsoNapja->zaras;
                    kimenetiFajl<<negyedDiff<<" "<<negyedDiff_ElsoNap<<" "; /// negyedDiff negyedDiff_ElsoNap


                    set<Nap>::iterator iterNap = negyedElsoNapja; // negyedIngadozas
                    float negyedMin = 1000000, negyedMax = 0;
                    float napCnt = 0, globMaxNapIndex = 0, globMinNapIndex = 0, globMaxNapErtek = 0, globMinNapErtek = 0;
                    float lokMaxCnt = 0, lastMaxErtek = iterNap->maximum, lastMaxIdx = 0, lastMinErtek = iterNap->maximum, lastMinIdx = 0;
                    vector<float> maximumok, minimumok;
                    cout<<"nap: ";
                    while (iterNap!=negyedUtolsoUtaniNapja){
                        if (iterNap->maximum>negyedMax){negyedMax=iterNap->maximum; globMaxNapIndex=napCnt;}
                        if (iterNap->minimum<negyedMin){negyedMin=iterNap->minimum; globMinNapIndex=napCnt;}
                        maximumok.push_back(iterNap->maximum);
                        minimumok.push_back(iterNap->minimum);
                        ++napCnt; ++iterNap;
                        if (napCnt>200) break;
                    }
                    cout<<napCnt<<endl;
                    //napCnt--;
                    float negyedAlso = min(negyedUtolsoNapja->zaras,negyedElsoNapja->nyitas);
                    float negyedFelso = max(negyedUtolsoNapja->zaras,negyedElsoNapja->nyitas);
                    kimenetiFajl<<negyedMax/negyedMin<<" "<<negyedAlso/negyedMin<<" "<<negyedMax/negyedFelso<<" "; /// negyedIngadozas negyedAlsoIngadozas negyedFelsoIngadozas

                    /// napCnt
                    kimenetiFajl<<napCnt<<" ";

                    /// negyedGlobMinHely negyedGlobMaxHely negyedGlobMinErtek negyedGlobMaxErtek
                    kimenetiFajl<<globMinNapIndex/napCnt<<" "<<globMaxNapIndex/napCnt<<" "<<napCnt<<" "<<negyedMin<<" "<<negyedMax<<" ";


                    /// negyedLokMinCnt15 negyedLokMaxCnt15

                    float negyedLokMinCnt = 0, negyedLokMaxCnt = 0;
                    for (float zzz=0; zzz<minimumok.size(); zzz++){
                        int start_i = max(0.0f,zzz-7), end_i = min(napCnt-1,zzz+7);
                        float lokMinPos = 0, lokMinErtek = 100000, lokMaxPos = 0, lokMaxErtek = 0;
                        for (int ki = start_i; ki<=end_i; ki++){
                            if (maximumok[ki]>lokMaxErtek){lokMaxErtek=maximumok[ki]; lokMaxPos=ki;}
                            if (minimumok[ki]<lokMinErtek){lokMinErtek=minimumok[ki]; lokMinPos=ki;}
                        }
                        if (lokMinPos==zzz){negyedLokMinCnt++;
                        //kimenetiFajl<<zzz<<"_n ";
                        }
                        if (lokMaxPos==zzz){negyedLokMaxCnt++;
                        //kimenetiFajl<<zzz<<"_x ";
                        }
                    }
                    kimenetiFajl<<negyedLokMinCnt<<" "<<negyedLokMaxCnt<<" ";

                    /*
                    iterNap = negyedElsoNapja;
                    while (iterNap!=negyedUtolsoUtaniNapja){
                        set<Nap>::iterator iterNapInNegyed = iterNap;
                        float lokMinPos = 0, lokMinErtek = 100000, lokMaxPos = 0, lokMaxErtek = 0;
                        int zzz=0;
                        for (; zzz<15 && iterNapInNegyed!=negyedUtolsoUtaniNapja; zzz++){
                            if (iterNapInNegyed->maximum>lokMaxErtek){lokMaxErtek=iterNapInNegyed->maximum; lokMaxPos=zzz;}
                            if (iterNapInNegyed->minimum<lokMinErtek){lokMinErtek=iterNapInNegyed->minimum; lokMinPos=zzz;}
                            ++iterNapInNegyed;
                        }
                        //if ()
                        ++iterNap;
                    }
                    */

                    kimenetiFajl<<endl;
                    elozo=negyed;
                }
                continue;
                //set<Nap>::iterator reszvenyNapjai = stocks[j].mindenNap.find(kezdonap);

                //continue;
                //Negyed n; n.
                //Nap kezdonap(2015,1,5);
                Nap kezdonap(2014,1,2);
                //Nap kezdonap(2023,1,9);
                //Nap kezdonap(2024,1,2);
                ///Datum vegNap(2024,1,1);
                Datum vegNap(2025,1,1);
                Nap ntemp(2023,10,18);
                if (stocks[j].mindenNap.find(ntemp) == stocks[j].mindenNap.end()) {
                    Nap n2temp = *(stocks[j].mindenNap.rbegin());
                    if (n2temp.datum.year==2023 && n2temp.datum.month==10 && n2temp.datum.day==13){}
                    else continue;
                    cout<<stocks[j].name<<" "<<n2temp.datum.year<<" "<<n2temp.datum.month<<" "<<n2temp.datum.day<<endl;

                }
                set<Nap>::iterator reszvenyNapjai = stocks[j].mindenNap.find(kezdonap);
                if (reszvenyNapjai==stocks[j].mindenNap.end()) continue;
                ofstream tempF("./reszvenyFajlok/"+stocks[j].name+".csv");
                //tempF<<"dátum,időpont,érték,második érték"<<endl;
                //tempF<<"dátum,időpont,érték"<<endl;

                float felfeleJobban = 0, osszesEset = 0, zarasLejebb = 0, zarasIsLejebb = 0;
                float profit = 1.0f, minNagyafter = 0 ,minNK = 0, startKisebb = 0;
                int nCnt = 0;
                //ofstream file(stocks[j].name+".txt");
                for (;reszvenyNapjai!=stocks[j].mindenNap.end() && reszvenyNapjai->datum<vegNap;){
                    ///cout<<j<<" "<<nCnt<<endl;
                    nCnt++;
                    //cout<<reszvenyNapjai->datum.day<<endl;;
                    Arfolyam perc(9,30);
                    Arfolyam percZ(15,59);
                    set<Arfolyam>::iterator kperc =  reszvenyNapjai->percek.find(perc);
                    set<Arfolyam>::iterator kpercZ =  reszvenyNapjai->percek.find(percZ);
                    if (reszvenyNapjai->percek.end()==kperc || reszvenyNapjai->percek.end()==kpercZ) continue;
                    float hanyPerc = 390;
                    vector<float> ertekek(hanyPerc+1); float startV;
                    vector<float> ertekek2(hanyPerc);
                    float szum = 0.0f;
                    for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kperc && cntPerc<hanyPerc; cntPerc++){
                        ertekek[cntPerc]=kperc->open;
                        ertekek2[cntPerc]=kperc->open;
                        szum+=ertekek[cntPerc];
                        startV=kperc->close;
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<kperc->open<<" "<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        kperc++;
                    }
                    ertekek[hanyPerc]=kpercZ->open;
                    szum+=ertekek[hanyPerc];


                    float min = *std::min_element(ertekek2.begin(), ertekek2.end());
                    float max = *std::max_element(ertekek2.begin(), ertekek2.end());

                    // Normalizálás
                    std::vector<float> normalized_data;
                    for(float value : ertekek) {
                        float normalized_value = (value - min) / (max - min);
                        normalized_data.push_back(normalized_value);
                    }

                    // Normalizált adatok kiírása
                    //std::cout << "Normalizált adatok: ";
                    kperc =  reszvenyNapjai->percek.find(perc);
                    for (int zzz=0; zzz<hanyPerc;zzz++){
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<normalized_data[zzz]<<","<<ertekek[zzz]<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<kperc->open<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        tempF<<kperc->open<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        kperc++;
                    }
                    ///tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kpercZ->idopont.ora<<":"<<kpercZ->idopont.perc<<","<<normalized_data[30]<<","<<ertekek[30]<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;

                    ///cout<<reszvenyNapjai->datum.year<<" "<<reszvenyNapjai->datum.month<<endl;
                    reszvenyNapjai++;
                    continue;
                    Arfolyam percK(10,0);
                    float endV = 0.0f;
                    set<Arfolyam>::iterator kpercK =  reszvenyNapjai->percek.find(percK);
                    if (reszvenyNapjai->percek.end()==kpercK) continue;
                    for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kpercK && cntPerc<358; cntPerc++){
                        endV = kpercK->close;
                        kpercK++;
                    }
                    reszvenyNapjai++;

                }
                //file.close();
                cout<<j<<", T: "<<clock()-t1<<endl;
            }

            sleep(1);
            /**
            cout<<"stabil: "<<fJ/oE*100.f<<" "<<zL/fJ*100.f<<" "<<zIL/zL*100.f<<" "<<zIL<<" / "<<zL<<" / "<<fJ<<" / "<<oE<<endl;
            cout<<"minden: "<<fJ2/oE2*100.f<<" "<<zL2/fJ2*100.f<<" "<<zIL2/zL2*100.f<<" "<<zIL2<<" / "<<zL2<<" / "<<fJ2<<" / "<<oE2<<endl;
            cout<<"stabil: "<<pF/z1<<", "<<f1/fJ<<" "<<f2/f3<<endl;
            cout<<"minden: "<<pF2/z2<<", "<<f4/fJ2<<" "<<f5/f6<<endl;
            */

        }
    }

    /// Adat lekérdezés 2
    if (false){
        vector<string> reszvenyekNeve = {"AVGO"};//csoportReszvenyei("estere");//{"NVDA"};//csoportReszvenyei("estere");
        reszvenyekNeve = csoportReszvenyei("osszes");

        for (int i=0; i<reszvenyekNeve.size();){
            for (;i<reszvenyekNeve.size(); i++)
                cout<<reszvenyekNeve[i]<<" ";
            cout<<endl;
        }
        //reszvenyekNeve.clear(); reszvenyekNeve.push_back("NFLX"); reszvenyekNeve.push_back("ADBE"); reszvenyekNeve.push_back("NVDA");
        cout<<reszvenyekNeve.size()<<endl;
        int thCnt = 32;
        vector<thread> szalak; szalak.resize(thCnt);
        vector<Stock> stocks; stocks.resize(thCnt);
        bool m;
        long long osszesPelda = 0, joPelda = 0;
        int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
        float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;


        ofstream kimenetiFajl("EgyNegyedEvKomplex.txt");
        for (size_t i=0; i<reszvenyekNeve.size();){
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
                cout<<stocks[j].name<<endl;

                Negyed elozo; elozo.income=-3;
                Negyed mostani; mostani.income=-3;
                for (const Negyed &negyed: stocks[j].negyedevek){
                    Negyed mostani=negyed;
                    if (elozo.income==-3){
                        elozo=mostani;
                        mostani=negyed;
                        continue;
                    }

                    /// ha kevés nap van a negyedév előtt TODO
                    stringstream egyRekord;

                    /// Vizsgált időszak:
                    if (!(Datum(2000,1,1) < negyed.korrigaltTenylegesJelentes)) continue;
                    if (!(negyed.korrigaltTenylegesJelentes < Datum(2010,1,1))) continue;

                    /// ellenőrző összeg: ha minden perc és minden nap megvan
                    int checkSum = 0;
                    bool baj = false;

                    /// Legyen 4 nap az utolsó negyedévben
                    if (!(negyed.korrigaltTenylegesJelentes < Datum(2024,9,4))) continue;

                    /// A két negyed nyitó napjai és a részvény neve
                    egyRekord<<stocks[j].name<<" ";
                    set<Nap>::iterator mostaniNegyedElsoNapja = stocks[j].mindenNap.find(elozo.korrigaltTenylegesJelentes);
                    set<Nap>::iterator kovetkezoNegyedElsoNapja = stocks[j].mindenNap.find(negyed.korrigaltTenylegesJelentes);
                    egyRekord<<mostaniNegyedElsoNapja->datum.year<<" "<<mostaniNegyedElsoNapja->datum.month<<" "<<mostaniNegyedElsoNapja->datum.day<<" ";
                    egyRekord<<kovetkezoNegyedElsoNapja->datum.year<<" "<<kovetkezoNegyedElsoNapja->datum.month<<" "<<kovetkezoNegyedElsoNapja->datum.day<<endl;

                    /// Az előző negyed utolsó 4 és a mostani negyed első 4 napja percbontásban (nyitas, min, max, zárás * 390)
                    set<Nap>::iterator mostaniNegyedMegelozoNapjai = mostaniNegyedElsoNapja;
                    for (int vissza=0; vissza<4; vissza++) {mostaniNegyedMegelozoNapjai--;}
                    for (int elore=0; elore<8; elore++){
                        for (set<Arfolyam>::iterator perc = mostaniNegyedMegelozoNapjai->percek.begin(); perc != mostaniNegyedMegelozoNapjai->percek.end(); ++perc){
                            if (perc->idopont.ora<9 || (perc->idopont.ora==9 && perc->idopont.perc<30)) continue;
                            if (perc->idopont.ora>15 || (perc->idopont.ora==9 && perc->idopont.perc<30)) continue;
                            egyRekord<<perc->open<<" "<<perc->minimum<<" "<<perc->maximum<<" "<<perc->close<<" ";
                            checkSum++;
                        }
                        egyRekord<<endl;
                        mostaniNegyedMegelozoNapjai++;
                    }
                    int chksum2 = checkSum;

                    /// A mostani negyedév minden napjának a 4 értéke (nyitas, min, max, zárás)
                    for (set<Nap>::iterator mostaniNegyedElsoNapjaCiklus = mostaniNegyedElsoNapja;
                        mostaniNegyedElsoNapjaCiklus != kovetkezoNegyedElsoNapja; mostaniNegyedElsoNapjaCiklus++){
                        egyRekord<<mostaniNegyedElsoNapjaCiklus->nyitas<<" "<<mostaniNegyedElsoNapjaCiklus->minimum<<" ";
                        egyRekord<<mostaniNegyedElsoNapjaCiklus->maximum<<" "<<mostaniNegyedElsoNapjaCiklus->zaras<<" ";
                    }
                    egyRekord<<endl;

                    /// Az mostani negyed utolsó 4 és a következő negyed első 4 napja percbontásban (nyitas, min, max, zárás * 390)
                    set<Nap>::iterator mostaniNegyedUtolsoNapjai = kovetkezoNegyedElsoNapja;
                    for (int vissza=0; vissza<4; vissza++) {mostaniNegyedUtolsoNapjai--;}
                    for (int elore=0; elore<8; elore++){
                        for (set<Arfolyam>::iterator perc = mostaniNegyedUtolsoNapjai->percek.begin(); perc != mostaniNegyedUtolsoNapjai->percek.end(); ++perc){
                            if (perc->idopont.ora<9 || (perc->idopont.ora==9 && perc->idopont.perc<30)) continue;
                            if (perc->idopont.ora>15 || (perc->idopont.ora==9 && perc->idopont.perc<30)) continue;
                            egyRekord<<perc->open<<" "<<perc->minimum<<" "<<perc->maximum<<" "<<perc->close<<" ";
                            checkSum++;
                        }
                        egyRekord<<endl;
                        mostaniNegyedUtolsoNapjai++;
                    }

                    egyRekord<<endl;
                    if (chksum2!=3120 || checkSum!=6240) {
                        cout<<stocks[j].name<<endl;
                        cout<<negyed.negyedevVege.year<<" "<<negyed.negyedevVege.month<<" "<<negyed.negyedevVege.day<<" "<<endl;
                        cout<<checkSum<<endl;
                    } else {

                        kimenetiFajl<<egyRekord.str();
                    }
                    elozo=mostani;
                    mostani=negyed;
                }
                continue;
                //set<Nap>::iterator reszvenyNapjai = stocks[j].mindenNap.find(kezdonap);

                //continue;
                //Negyed n; n.
                //Nap kezdonap(2015,1,5);
                Nap kezdonap(2014,1,2);
                //Nap kezdonap(2023,1,9);
                //Nap kezdonap(2024,1,2);
                ///Datum vegNap(2024,1,1);
                Datum vegNap(2025,1,1);
                Nap ntemp(2023,10,18);
                if (stocks[j].mindenNap.find(ntemp) == stocks[j].mindenNap.end()) {
                    Nap n2temp = *(stocks[j].mindenNap.rbegin());
                    if (n2temp.datum.year==2023 && n2temp.datum.month==10 && n2temp.datum.day==13){}
                    else continue;
                    cout<<stocks[j].name<<" "<<n2temp.datum.year<<" "<<n2temp.datum.month<<" "<<n2temp.datum.day<<endl;

                }
                set<Nap>::iterator reszvenyNapjai = stocks[j].mindenNap.find(kezdonap);
                if (reszvenyNapjai==stocks[j].mindenNap.end()) continue;
                ofstream tempF("./reszvenyFajlok/"+stocks[j].name+".csv");
                //tempF<<"dátum,időpont,érték,második érték"<<endl;
                //tempF<<"dátum,időpont,érték"<<endl;

                float felfeleJobban = 0, osszesEset = 0, zarasLejebb = 0, zarasIsLejebb = 0;
                float profit = 1.0f, minNagyafter = 0 ,minNK = 0, startKisebb = 0;
                int nCnt = 0;
                //ofstream file(stocks[j].name+".txt");
                for (;reszvenyNapjai!=stocks[j].mindenNap.end() && reszvenyNapjai->datum<vegNap;){
                    ///cout<<j<<" "<<nCnt<<endl;
                    nCnt++;
                    //cout<<reszvenyNapjai->datum.day<<endl;;
                    Arfolyam perc(9,30);
                    Arfolyam percZ(15,59);
                    set<Arfolyam>::iterator kperc =  reszvenyNapjai->percek.find(perc);
                    set<Arfolyam>::iterator kpercZ =  reszvenyNapjai->percek.find(percZ);
                    if (reszvenyNapjai->percek.end()==kperc || reszvenyNapjai->percek.end()==kpercZ) continue;
                    float hanyPerc = 390;
                    vector<float> ertekek(hanyPerc+1); float startV;
                    vector<float> ertekek2(hanyPerc);
                    float szum = 0.0f;
                    for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kperc && cntPerc<hanyPerc; cntPerc++){
                        ertekek[cntPerc]=kperc->open;
                        ertekek2[cntPerc]=kperc->open;
                        szum+=ertekek[cntPerc];
                        startV=kperc->close;
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<kperc->open<<" "<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        kperc++;
                    }
                    ertekek[hanyPerc]=kpercZ->open;
                    szum+=ertekek[hanyPerc];


                    float min = *std::min_element(ertekek2.begin(), ertekek2.end());
                    float max = *std::max_element(ertekek2.begin(), ertekek2.end());

                    // Normalizálás
                    std::vector<float> normalized_data;
                    for(float value : ertekek) {
                        float normalized_value = (value - min) / (max - min);
                        normalized_data.push_back(normalized_value);
                    }

                    // Normalizált adatok kiírása
                    //std::cout << "Normalizált adatok: ";
                    kperc =  reszvenyNapjai->percek.find(perc);
                    for (int zzz=0; zzz<hanyPerc;zzz++){
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<normalized_data[zzz]<<","<<ertekek[zzz]<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        //tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kperc->idopont.ora<<":"<<kperc->idopont.perc<<","<<kperc->open<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        tempF<<kperc->open<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                        kperc++;
                    }
                    ///tempF<<reszvenyNapjai->datum.year<<"-"<<reszvenyNapjai->datum.month<<"-"<<reszvenyNapjai->datum.day<<","<< kpercZ->idopont.ora<<":"<<kpercZ->idopont.perc<<","<<normalized_data[30]<<","<<ertekek[30]<<endl;//<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;

                    ///cout<<reszvenyNapjai->datum.year<<" "<<reszvenyNapjai->datum.month<<endl;
                    reszvenyNapjai++;
                    continue;
                    Arfolyam percK(10,0);
                    float endV = 0.0f;
                    set<Arfolyam>::iterator kpercK =  reszvenyNapjai->percek.find(percK);
                    if (reszvenyNapjai->percek.end()==kpercK) continue;
                    for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kpercK && cntPerc<358; cntPerc++){
                        endV = kpercK->close;
                        kpercK++;
                    }
                    reszvenyNapjai++;

                }
                //file.close();
                cout<<j<<", T: "<<clock()-t1<<endl;
            }

            sleep(1);

        }

        kimenetiFajl.close();
    }


    /// Egy speciális eset
    if (false){
    vector<string> reszvenyekNeve = csoportReszvenyei("estere"); //osszesReszveny();{"NVDA","INTC","AAPL","TSLA","AMD"};
    //reszvenyekNeve = osszesReszveny();
    for (int i=0; i<reszvenyekNeve.size();){
        for (;i<reszvenyekNeve.size(); i++)
            cout<<reszvenyekNeve[i]<<" ";
        cout<<endl;
    }
    //reszvenyekNeve.clear(); reszvenyekNeve.push_back("NFLX"); reszvenyekNeve.push_back("ADBE"); reszvenyekNeve.push_back("NVDA");
    cout<<reszvenyekNeve.size()<<endl;
    int thCnt = 32;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<Stock> stocks; stocks.resize(thCnt);
    bool m;
    long long osszesPelda = 0, joPelda = 0;
    int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
    float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;


    cout<<"alma"<<endl;
    vector<vector<array<array<float,80>,435>>> kombinaciokProfitja;
    cout<<"alma"<<endl;
    if (true){
        array<float,80> basic = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
        array<array<float,80>,435> vec3; for (int ic=0; ic<435; ic++) vec3[ic]=basic;
        vector<array<array<float,80>,435>> vec4; for (int ic=0; ic<435; ic++) vec4.push_back(vec3);
        for (int ic=0; ic<435; ic++) kombinaciokProfitja.push_back(vec4);
    }

    vector<array<array<bool,435>,435>> kombinaciok;
    if (true){
        bool basic = false;
        array<bool,435> vec3; for (int ic=0; ic<435; ic++) vec3[ic]=basic;
        array<array<bool,435>,435> vec4; for (int ic=0; ic<435; ic++) vec4[ic]=vec3;
        for (int ic=0; ic<435; ic++) kombinaciok.push_back(vec4);
    }
    cout<<"alma"<<endl;


    float zL = 0, zIL = 0, oE = 0, fJ = 0, pF = 1.0f;
    float zL2 = 0, zIL2 = 0, oE2 = 0, fJ2 = 0, pF2 = 1.0f;
    for (size_t i=0; i<reszvenyekNeve.size();){
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
            //Nap kezdonap(2015,1,5);
            Nap kezdonap(2014,1,2);
            //Nap kezdonap(2023,1,9);
            //Nap kezdonap(2024,1,2);
            Datum vegNap(2024,1,1);
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
            ofstream file(stocks[j].name+".txt");
            for (;reszvenyNapjai!=stocks[j].mindenNap.end() && reszvenyNapjai->datum<vegNap;){
                ///cout<<j<<" "<<nCnt<<endl;
                nCnt++;
                //cout<<reszvenyNapjai->datum.day<<endl;;
                Arfolyam perc(9,30);
                Arfolyam percZ(15,58);
                set<Arfolyam>::iterator kperc =  reszvenyNapjai->percek.find(perc);
                set<Arfolyam>::iterator kpercZ =  reszvenyNapjai->percek.find(percZ);
                if (reszvenyNapjai->percek.end()==kperc || reszvenyNapjai->percek.end()==kpercZ) continue;
                float ertekek[390], startV;
                for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kperc && cntPerc<390; cntPerc++){
                    ertekek[cntPerc]=kperc->open;
                    startV=kperc->close;
                    file<<reszvenyNapjai->datum.year<<" "<<reszvenyNapjai->datum.month<<" "<<reszvenyNapjai->datum.day<<" "<< kperc->idopont.ora<<" "<<kperc->idopont.perc<<" "<<kperc->open<<" "<<kperc->close<<" "<<kperc->minimum<<" "<<kperc->maximum<<endl;
                    kperc++;
                }
                cout<<reszvenyNapjai->datum.year<<" "<<reszvenyNapjai->datum.month<<endl;
                reszvenyNapjai++;
                continue;
                Arfolyam percK(10,0);
                float endV = 0.0f;
                set<Arfolyam>::iterator kpercK =  reszvenyNapjai->percek.find(percK);
                if (reszvenyNapjai->percek.end()==kpercK) continue;
                for (int cntPerc = 0;reszvenyNapjai->percek.end()!=kpercK && cntPerc<358; cntPerc++){
                    endV = kpercK->close;
                    kpercK++;
                }


                vector<bool> kombok(435);
                int icC = 0;
                for (int ic1=0; ic1<30; ic1++){
                    for (int ic2=ic1+1; ic2<30; ic2++){
                        //if (ertekek[ic1]<ertekek[ic2]) {kombok[icC]=true; kombok[icC+435]=false; }
                        //else {kombok[icC]=false; kombok[icC+435]=true; }
                        kombok[icC]=(ertekek[ic1]<ertekek[ic2]);
                        icC++;
                    }
                }
                //cout<<icC<<endl;

                vector<thread> ths(thCnt);
                int kapottSorok = 435/thCnt+1;
                for (int ic1=0; ic1<thCnt; ic1+=1){
                    //cout<<ic1<<endl;
                    ths[ic1] = thread(iterFV,ic1*kapottSorok,ref(kombinaciok),ref(kombinaciokProfitja),ref(kombok),startV,endV,kapottSorok,reszvenyNapjai->hetMelyikNapja);

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
                for (int ic1=0; ic1<thCnt; ic1+=1){
                    ths[ic1].join();
                }


                reszvenyNapjai++;

            }
            file.close();
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

    clock_t t1 = clock();
    ofstream of("kimenet.txt");

        for(int k1=0;k1<435;k1++){
            cout<<"k1: "<<k1<<endl;
            for(int k2=0;k2<435;k2++){
                for(int k3=0;k3<435;k3++){
                    for(int k4=0;k4<80;k4+=2){
                    of<<kombinaciokProfitja[k1][k2][k3][k4]<<" "<<kombinaciokProfitja[k1][k2][k3][k4+1]<<" ";
                }
            }
            of<<endl;
        }
        of<<endl<<endl;
    }

        cout<<"almak"<<endl;
    of.close();
    cout<<"T: "<<clock()-t1<<endl;

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
