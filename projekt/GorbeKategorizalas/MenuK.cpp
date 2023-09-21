#include "Menu.h"

MenuK::MenuK(SDL_Pack sdlp, Menu **act){
    fomenu = FoMenu(sdlp,act);
    frissitomenu = FrissitoMenu(sdlp,act);

    fomenu.nextMenus(&frissitomenu);
    frissitomenu.nextMenus(&fomenu);


    startMenu = &fomenu;
    *act = startMenu;
}
