#include "Menu.h"

void CsoportEditorMenu::nextMenus(Menu *fo, Menu *friss){
    fomenu=fo;
    frissitomenu=friss;
}

void CsoportEditorMenu::draw() {
    SDL_SetRenderDrawColor(renderer,100,200,20,255);
    SDL_RenderClear(renderer);
    stringRGBA(renderer,20,20,"barack",126,250,20,255);
    SDL_RenderPresent(renderer);
}

void CsoportEditorMenu::inputHandle() {

    if (SDL_PollEvent(ev)){
        if (ev->type==SDL_MOUSEBUTTONDOWN){
        } else if (ev->type==SDL_MOUSEBUTTONUP){
        } else if (ev->type==SDL_MOUSEMOTION){
            //MX = ev->motion.x;
            //MY = ev->motion.y;
        } else if (ev->type==SDL_KEYDOWN){
            if (ev->key.keysym.sym==SDLK_r){
                cout<<"CsoportEditorMenu"<<endl;
            }
            if (ev->key.keysym.sym==SDLK_d){
                cout<<"change"<<endl;
                *menu = fomenu;
            }
            if (ev->key.keysym.sym==SDLK_f){
                cout<<"change"<<endl;
                *menu = frissitomenu;
            }
        }
        if (ev->type == SDL_QUIT)
            exit(3);
    }
}
