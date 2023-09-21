#include "common.h"
#define FONT "LiberationSerif-Regular.ttf"

using namespace std;

int main( int argc, char * argv[] )
{

    /// Inicializálás
    srand(time(NULL));
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer( SZELES, MAGAS,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_RENDERER_ACCELERATED
                                , &window, &renderer );
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear( renderer );
    SDL_RenderPresent( renderer );

    main2( *window, *renderer);
    return 0;
}
