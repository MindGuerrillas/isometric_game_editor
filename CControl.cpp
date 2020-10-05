#include "CControl.h"

std::vector<CControl*>  CControl::ControlList;

CControl::CControl()
{
    X = Y = W = H = 0;
    SControlSurface = NULL;
}

CControl::~CControl()
{
    
}

bool CControl::Load(SDL_Surface* SDisplay)
{
    SControlSurface = SDL_CreateRGBSurface(SDL_HWSURFACE,W,H,32,SDisplay->format->Rmask,SDisplay->format->Gmask,SDisplay->format->Bmask,SDisplay->format->Amask);
    
    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.w = W;
    Rect.h = H;
    
    SDL_FillRect(SControlSurface, &Rect, 0xdedede);
}

void CControl::Render(SDL_Surface* SDisplay)
{
    
    CSurface::Draw(SDisplay, SControlSurface, X, Y); 
}

void CControl::Tidy()
{
    if(SControlSurface)
        SDL_FreeSurface(SControlSurface);
}

void CControl::OnLButtonUp(int x, int y)
{    
    // Is click on the control?
    if ((x >= this->X && x <= this->X + W ) && (y >= this->Y && y <= this->Y + H) )
    {
        /*
        // Highlight the current tile
        int newx, newy;
        newx = x - this->X;
        newy = y - this->Y;
        
        unsigned int ID = 0;
       // ID = newx / TILE_SIZE;
        //ID = ID + (TilesetWidth * (newy / TILE_SIZE));
        
        *SelectedTile = ID;
         * */
        
        OnClick(x - this->X, y - this->Y);
    }
}

void CControl::OnClick(int x, int y)
{
    
}