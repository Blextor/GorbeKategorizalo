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

void megjelenites(SDL_Renderer &renderer,/* vector<vector<Mezo>> &mezok,*/ SDL_Window &window){
    while (true){
        clock_t t1;
        clock_t delay=60;
        if (clock()>t1+CLOCKS_PER_SEC/delay){
            //cout<<"alma"<<clock()<<endl;
            if (kirajzol){
                draw(renderer);
                cout<<"a";
                //Draw_Mezok(renderer, mezok, window);
            }
            t1=clock();
        } else {
            Sleep(1);
        }
    }
}

void esemenyKezel(thread &frame,SDL_Renderer &renderer, SDL_Window &window){
    SDL_Event ev;
    while (true){
        if (SDL_PollEvent(&ev)){
            if (ev.window.event == SDL_WINDOWEVENT_RESIZED){
                cout<<"resized"<<endl;
                //thread frame2(megjelenites,ref(renderer),ref(window));
                //frame = thread(megjelenites,ref(renderer),ref(window));
            }
            if (ev.type==SDL_MOUSEBUTTONDOWN){
            } else if (ev.type==SDL_MOUSEBUTTONUP){
            } else if (ev.type==SDL_MOUSEMOTION){
                MX = ev.motion.x;
                MY = ev.motion.y;
            } else if (ev.type==SDL_KEYDOWN){
                if (ev.key.keysym.sym==SDLK_r){
                    cout<<"calma"<<endl;
                }

                if (ev.key.keysym.sym==SDLK_d){
                    kirajzol=!kirajzol;
                }

                if (ev.key.keysym.sym==SDLK_b){
                }
                if (ev.key.keysym.sym==SDLK_n){
                }

                if (ev.key.keysym.sym==SDLK_j){
                }

                if (ev.key.keysym.sym==SDLK_k){
                }

                if (ev.key.keysym.sym==SDLK_s){
                }

                if (ev.key.keysym.sym==SDLK_p){
                }

                if (ev.key.keysym.sym==SDLK_m){
                }

                if (ev.key.keysym.sym==SDLK_l){
                }
            }
            if (ev.type == SDL_QUIT)
                exit(3);
        }
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


    /// adatokat kell beolvasnom


    int fps = 0;
    clock_t f1 = 0, f2 = 0, f3 = 0, f4 = 0;

    float szum=1.0f;

    LokMinMax m;
    Cimke c;
    Idosor ids;
    m.azIdosorIlyen(ids);
    c.azIdosorIlyen(ids);

    std::vector<Cimke*> animals;

    animals.push_back(&m);
    //animals.push_back(std::make_unique<Cat>());
    animals.push_back(&c);

    for (const auto& animal : animals) {
        cout<<animal->azIdosorIlyen(ids);
    }


    cout<<m.type<<c.type<<endl;


    SDL_Pack sdlp(&window,&renderer,&ev);
    Menu *actualMenu;
    MenuK menuk(sdlp,&actualMenu);

    //thread frame(megjelenites,ref(renderer),ref(window));   /// megjelenítás
    //thread esemeny(esemenyKezel,ref(frame),ref(renderer),ref(window));                           /// SDL input feldolgozás
    //thread konzol(konzolKezel);                             /// konzol input feldolgozás
    while(true){
        actualMenu->draw();
        actualMenu->inputHandle();
        //SDL_PollEvent(&ev);                                 /// Ez kell az input feldolgozáshoz külön szálon
        //if (!kirajzol){}
          //  draw(renderer);
        Sleep(1);
        //ev.window.event
        //cout<<ev.window.event<<endl;
        if (ev.window.event == SDL_WINDOWEVENT_RESIZED){
            cout<<"resized"<<endl;
            int x, y;
            SDL_GetWindowSize(&window,&x,&y);
            cout<<x<<" "<<y<<endl;
            //frame = thread(megjelenites,ref(renderer),ref(window));
        }
    }
}
