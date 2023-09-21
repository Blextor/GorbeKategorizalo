#include "Menu.h"

void FrissitoMenu::nextMenus(Menu *fo){
    fomenu=fo;
}

void FrissitoMenu::draw() {
    SDL_SetRenderDrawColor(renderer,100,20,200,255);
    SDL_RenderClear(renderer);
    stringRGBA(renderer,20,20,"körte",126,250,20,255);
    SDL_RenderPresent(renderer);
}

void FrissitoMenu::inputHandle() {
    if (SDL_PollEvent(ev)){
        if (ev->type==SDL_MOUSEBUTTONDOWN){
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        } else if (ev->type==SDL_KEYDOWN){
            if (ev->key.keysym.sym==SDLK_r){
                cout<<"FrissitoMenu"<<endl;
            }
            if (ev->key.keysym.sym==SDLK_d){
                cout<<"change"<<endl;
                *menu = fomenu;
            }
        }
        if (ev->type == SDL_QUIT)
            exit(3);
    }
}
