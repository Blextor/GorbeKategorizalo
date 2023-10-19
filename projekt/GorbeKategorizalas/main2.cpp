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

    vector<string> reszvenyekNeve = osszesReszveny();
    int thCnt = 10;
    vector<thread> szalak; szalak.resize(thCnt);
    vector<Stock> stocks; stocks.resize(thCnt);
    long long osszesPelda = 0, joPelda = 0;
    for (size_t i=0; i<reszvenyekNeve.size(); i++){
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

            set<Negyed>::iterator nulladik, elso, masodik,harmadik,negyedik;
            nulladik = stocks[j].negyedevek.begin();
            elso = ++nulladik;
            masodik = ++nulladik;
            harmadik = ++nulladik;
            negyedik = ++nulladik;
            --nulladik; --nulladik; --nulladik; --nulladik;
            for (;negyedik!=stocks[j].negyedevek.end();++nulladik, ++elso, ++masodik, ++harmadik, ++negyedik){
                float e1=0, e2=0;
                Negyed negyed0=(*nulladik);
                Negyed negyed1=(*elso);
                Negyed negyed2=(*masodik);
                Negyed negyed3=(*harmadik);
                Negyed negyed4=(*negyedik);
                if (negyed3.korrigaltTenylegesJelentes<Datum(2019,1,1)){
                    continue;
                }
                else {
                    //break;
                }

                stocks[j].getNyit(e1,negyed0,true);
                stocks[j].getZar(e2,negyed0,true);
                if (e1>=e2) {cout<<"";continue;} /// 0. negyed nyit < 0. negyed zar
                stocks[j].getNyit(e1,negyed1,true);
                stocks[j].getZar(e2,negyed1,true);
                if (e1>=e2) {cout<<"";continue;} /// 1. negyed nyit < 1. negyed zar
                stocks[j].getNyit(e1,negyed2,true);
                stocks[j].getZar(e2,negyed2,true);
                if (e1>=e2) {cout<<"";continue;} /// 2. negyed nyit < 2. negyed zar
                stocks[j].getNyit(e1,negyed3,true);
                stocks[j].getZar(e2,negyed3,true);
                if (e1<=e2) {cout<<"";continue;} /// 3. negyed nyit > 3. negyed zar



                stocks[j].getNyit(e1,negyed0,true);
                stocks[j].getZar(e2,negyed3,true);
                if (e1>=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar
                stocks[j].getZar(e1,negyed0,true);
                stocks[j].getZar(e2,negyed2,true);
                if (e1>=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar
                stocks[j].getNyit(e1,negyed1,true);
                stocks[j].getZar(e2,negyed3,true);
                if (e1>=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar

                stocks[j].getNyit(e1,negyed2,true);
                stocks[j].getZar(e2,negyed3,true);
                if (e1<=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar
                stocks[j].getZar(e1,negyed2,true);
                stocks[j].getNyit(e2,negyed3,true);
                if (e1<=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar

                stocks[j].getMax(e1,negyed2,true);
                stocks[j].getMax(e2,negyed3,true);
                if (e1<=e2) {cout<<"";continue;} /// 0. negyed nyit > 0. negyed zar
                /*

                /*
                */

                osszesPelda++;

                cout<<stocks[j].name<<" "<<negyed4.korrigaltTenylegesJelentes.year<<" "<<
                negyed4.korrigaltTenylegesJelentes.month<<" "<<
                negyed4.korrigaltTenylegesJelentes.day<<endl;

                stocks[j].getZar(e1,negyed3,true);
                stocks[j].getNyit(e2,negyed4,true);
                if (e1>=e2) continue; /// 2. negyed zar > 3. negyed nyit
                cout<<"siker"<<endl;
                joPelda++;
            }
        }
        cout<<"osszes, jo: "<<osszesPelda<<" "<<joPelda<<" "<<(float)joPelda/osszesPelda<<endl;

    }
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

    /// megjelen�t�s m�g kicsit hib�s, nem tudom k�l�n sz�lon futtatni
    //thread frame(megjelenites,ref(actualMenu));   /// megjelen�t�s
    //thread esemeny(esemenyKezel,ref(actualMenu)); /// SDL input feldolgoz�s
    //thread konzol(konzolKezel);                             /// konzol input feldolgoz�s
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
        //SDL_PollEvent(&ev);                                 /// Ez kell az input feldolgoz�shoz k�l�n sz�lon
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
