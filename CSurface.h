
// Surface functions

#ifndef _CSURFACE_H_
    #define _CSURFACE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class CSurface 
{
    public:
        CSurface();

    public:
        static SDL_Surface* Load(std::string File, bool alpha);

        static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);

        static bool Draw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y, int X2, int Y2, int W, int H);

        static bool Transparent(SDL_Surface* Surf_Dest, int R, int G, int B);
        
        static Uint32 getpixel(SDL_Surface *surface, int x, int y);
};

#endif
