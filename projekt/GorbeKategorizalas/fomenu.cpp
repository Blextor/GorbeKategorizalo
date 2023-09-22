#include "Menu.h"

void FoMenu::nextMenus(Menu *frissito, Menu *csoport){
    frissitoMenu=frissito;
    csoportEditormenu=csoport;
}

void FoMenu::draw() {
    SDL_SetRenderDrawColor(renderer,100,20,20,255);
    SDL_RenderClear(renderer);
    stringRGBA(renderer,20,20,"almafa",126,250,20,255);
    SDL_RenderPresent(renderer);
}

void FoMenu::inputHandle(){
    if (SDL_PollEvent(ev) || true){
        if (ev->type==SDL_MOUSEBUTTONDOWN){
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        } else if (ev->type==SDL_KEYDOWN){
            if (ev->key.keysym.sym==SDLK_r){
                cout<<"FoMenu"<<endl;
            }
            if (ev->key.keysym.sym==SDLK_d){
                cout<<"change"<<endl;
                *menu = frissitoMenu;
            }
            if (ev->key.keysym.sym==SDLK_f){
                cout<<"change"<<endl;
                *menu = csoportEditormenu;
            }
        }
        if (ev->type == SDL_QUIT)
            exit(3);
    }
}

void FoMenu::process(){

}


