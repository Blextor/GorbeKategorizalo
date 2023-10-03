#include "Menu.h"

MenuK::MenuK(SDL_Pack sdlp, Menu **act){
    fomenu = FoMenu(sdlp,act);
    frissitomenu = FrissitoMenu(sdlp,act);
    csoportEditormenu = CsoportEditorMenu(sdlp,act);
    reszvenyMenu = ReszvenyMenu(sdlp,act);

    fomenu.nextMenus(&frissitomenu,&csoportEditormenu,&reszvenyMenu);
    frissitomenu.nextMenus(&fomenu,&csoportEditormenu);
    csoportEditormenu.nextMenus(&fomenu,&frissitomenu);
    reszvenyMenu.nextMenus(&fomenu);


    Stock stock;


    startMenu = &fomenu;
    *act = startMenu;
}
