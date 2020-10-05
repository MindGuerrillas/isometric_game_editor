
#ifndef _CTILESET_H_
    #define _CTILESET_H_

#include <SDL.h>
#include <vector>
#include "CTile.h"
#include "CSurface.h"
#include "CEvent.h"
#include "CControl.h"

class CTileset : public CControl
{
    public:
  
        SDL_Surface*            STileset;                
        SDL_Surface*            STile_Grid;       
        SDL_Surface*            STile_Highlight;
        
    private:        
        int TilesetWidth;
       
    public:
        CTileset();

        bool Load(SDL_Surface* SDisplay);

        void Render(SDL_Surface* SDisplay);

        void Tidy();
        void OnClick(int x, int y);
        
        int     SelectedTile;
};

#endif
