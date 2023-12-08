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

    /*
    Stock stock11,stock22,stock33,stock44,stock55;
    thread th1(loadStock,"NVDA",ref(stock11));
    thread th2(loadStock,"AMD",ref(stock22));
    thread th3(loadStock,"NFLX",ref(stock33));
    thread th4(loadStock,"AA",ref(stock44));
    thread th5(loadStock,"MCD",ref(stock55));
    th1.join();th2.join();th3.join();th4.join();th5.join();
    */

    vector<string> reszvenyekNeve = csoportReszvenyei("estere"); //osszesReszveny();
    reszvenyekNeve = osszesReszveny();
    int thCnt = 10;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<Stock> stocks; stocks.resize(thCnt);
    long long osszesPelda = 0, joPelda = 0;
    int z1 = 0, z2 = 0, z3 = 0, z4 = 0, z5 = 0, z6 = 0;
    float f1 = 0, f2 = 0, f3 = 0, f4 = 0, f5 = 0, f6 = 0;
    for (size_t i=1000; i<reszvenyekNeve.size(); i++){
        for (int j=0; j<thCnt; j++){
            szalak[j] = thread(loadStock,reszvenyekNeve[i],ref(stocks[j]));
            i++;
            //cout<<i<<endl;
            if (i>=reszvenyekNeve.size()) break;
        }


        for (int j=0; j<thCnt; j++){
            if (szalak[j].joinable())
                szalak[j].join();

            if (stocks[j].negyedevek.size()<3) continue;
            Nap ntemp(2023,10,18);
            if (stocks[j].mindenNap.find(ntemp) == stocks[j].mindenNap.end()) {
                Nap n2temp = *(stocks[j].mindenNap.rbegin());
                if (n2temp.datum.year==2023 && n2temp.datum.month==10 && n2temp.datum.day==13){}
                else continue;
                cout<<stocks[j].name<<" "<<n2temp.datum.year<<" "<<n2temp.datum.month<<" "<<n2temp.datum.day<<endl;

            }
            set<Negyed>::iterator n0, n1;
            n0 = stocks[j].negyedevek.begin();
            n1=++n0;
            n0--;

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
                if (e1>e5) continue; e1=e5;
                stocks[j].getMin(e5,d1,true);
                if (e2<e5) continue; e2=e5;
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

                /*
                stocks[j].getZar(e5,d1,true); stocks[j].getZar(e6,d1,true); if (e5>e4) z1++;
                stocks[j].getZar(e5,d2,true); stocks[j].getZar(e6,d2,true); if (e5>e4) z2++;
                stocks[j].getZar(e5,d3,true); stocks[j].getZar(e6,d3,true); if (e5>e4) z3++;
                stocks[j].getZar(e5,d4,true); stocks[j].getZar(e6,d4,true); if (e5>e4) z4++;
                stocks[j].getZar(e5,d5,true); stocks[j].getZar(e6,d5,true); if (e5>e4) z5++;
                stocks[j].getZar(e5,d6,true); stocks[j].getZar(e6,d6,true); if (e5>e4) z6++;
                */
                cout<<"alma "<<stocks[j].name<<" "<<dv.datum.year<<" "<<dv.datum.month<<" "<<dv.datum.day<<endl;
                /// Helyes elmozdulás
                if (dv.nyitas<e4) continue;
                joPelda++;
                //cout<<"osszes, jo: "<<osszesPelda<<" "<<joPelda<<" "<<(float)joPelda/osszesPelda<<endl;

            }
            cout<<i+j<<endl;
        }

    }
    cout<<z1<<" "<<z2<<" "<<z3<<" "<<z4<<" "<<z5<<" "<<z6<<endl;
    if (osszesPelda>0)cout<<z1*100/osszesPelda<<" "<<z2*100/osszesPelda<<" "<<z3*100/osszesPelda<<" "<<z4*100/osszesPelda<<" "<<z5*100/osszesPelda<<" "<<z6*100/osszesPelda<<endl;
    cout<<"osszes, jo: "<<osszesPelda<<" "<<joPelda<<" "<<(float)joPelda/osszesPelda<<endl;




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


    SDL_Pack sdlp(&window,&renderer,&ev);
    Menu *actualMenu;
    MenuK menuk(sdlp,&actualMenu);
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
