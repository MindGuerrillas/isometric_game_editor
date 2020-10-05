
#include "CSurface.h"

CSurface::CSurface() 
{
    
}

SDL_Surface* CSurface::Load(std::string File, bool alpha) 
{    
    SDL_Surface* STemp = NULL;
    SDL_Surface* SReturn = NULL;
    
    if((STemp = IMG_Load(File.c_str())) == NULL)
        return NULL;
    
    if (alpha)
        SReturn = SDL_DisplayFormatAlpha(STemp);
    else
    {
        SReturn = SDL_DisplayFormat(STemp);
        
        // Set Transparency color to RGB(255,0,255)
        Transparent(SReturn, 255, 0, 255);
    }
    
    SDL_FreeSurface(STemp);
    STemp = NULL;
    
    return SReturn;
}

bool CSurface::Draw(SDL_Surface* SDest, SDL_Surface* SSrc, int X, int Y) 
{
    if(SDest == NULL || SSrc == NULL)
        return false;
    
    SDL_Rect DestR;

    DestR.x = X;
    DestR.y = Y;

    SDL_BlitSurface(SSrc, NULL, SDest, &DestR);

    return true;
}


bool CSurface::Draw(SDL_Surface* SDest, SDL_Surface* SSrc, int X, int Y, int X2, int Y2, int W, int H) 
{
    if(SDest == NULL || SSrc == NULL) 
        return false;
    
    SDL_Rect DestR;

    DestR.x = X;
    DestR.y = Y;

    SDL_Rect SrcR;

    SrcR.x = X2;
    SrcR.y = Y2;
    SrcR.w = W;
    SrcR.h = H;

    SDL_BlitSurface(SSrc, &SrcR, SDest, &DestR);

    return true;
}

bool CSurface::Transparent(SDL_Surface* SDest, int R, int G, int B) 
{
    if(SDest == NULL) 
        return false;
   
    SDL_SetColorKey(SDest, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(SDest->format, R, G, B));

    return true;
}


Uint32 CSurface::getpixel(SDL_Surface *surface, int x, int y)
{
    SDL_LockSurface(surface);
    
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    SDL_UnlockSurface(surface);
    
    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}