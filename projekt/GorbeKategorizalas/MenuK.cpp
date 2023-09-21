#include "Menu.h"

MenuK::MenuK(SDL_Pack sdlp, Menu **act){
    fomenu = FoMenu(sdlp,act);
    frissitomenu = FrissitoMenu(sdlp,act);
    csoportEditormenu = CsoportEditorMenu(sdlp,act);

    fomenu.nextMenus(&frissitomenu,&csoportEditormenu);
    frissitomenu.nextMenus(&fomenu,&csoportEditormenu);
    csoportEditormenu.nextMenus(&fomenu,&frissitomenu);


    startMenu = &fomenu;
    *act = startMenu;
}
