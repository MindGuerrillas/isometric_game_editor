#include <SDL/SDL_keysym.h>
#include "CGame.h"
#include "CDebugLogging.h"
#include "CMap.h"

void CGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode) 
{    
    switch(sym) 
    {            
        case SDLK_LEFT:
            CCamera::Camera.OnMove(-CAMERA_SCROLL_INC, 0);
            break;
            
        case SDLK_RIGHT:
            CCamera::Camera.OnMove(CAMERA_SCROLL_INC, 0);
            break;
            
        case SDLK_UP:
            CCamera::Camera.OnMove(0, -CAMERA_SCROLL_INC);
            break;
            
        case SDLK_DOWN:
            CCamera::Camera.OnMove(0, CAMERA_SCROLL_INC);
            break;
            
        default: {
        }
    }
}

void CGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode) 
{

    switch(sym) 
    {
        case SDLK_g:
            CMap::Map.ShowGrid = !CMap::Map.ShowGrid;
            break;

        case SDLK_c:
            CMap::Map.AutoCoast = !CMap::Map.AutoCoast;
            break;
            
        case SDLK_s:
            CMap::Map.Save();
            break;
            
        default: {
        }
    }
}

void CGame::OnUser(Uint8 type, int code, void* data1, void* data2) 
{
    // USER EVENTS
   
    switch (code)
    {
        case GAME_EVENT_RESET:
            Reset();
            break;
            
        case GAME_EVENT_LEVEL_COMPLETE:
            //LevelComplete();
            break;
            
        case GAME_EVENT_GAMEOVER:
            GameOver();
            break;
    }
    
}

void CGame::OnLButtonDown(int x, int y)
{
    CTile* Tile = CMap::Map.GetTileByCoord(x, y - TILE_HEIGHT);
    
    if (Tile == NULL)
        return;
    
    if (y > TILE_HEIGHT)
        CMap::Map.DrawTile(Tile, TileSelector.SelectedTile);            
}

void CGame::OnRButtonDown(int x, int y)
{
    CTile* Tile = CMap::Map.GetTileByCoord(x,y - TILE_HEIGHT);
    
    if (Tile == NULL)
        return;

   // Check that it's on the Map
    if (y > TILE_HEIGHT)
    {
        
    }   
}

void CGame::OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle)
{
    if ((mY - TILE_HEIGHT) <= TILE_HEIGHT)
        return;
    
    CTile*  CurrentTile = CMap::Map.GetTileByCoord(mX, mY - TILE_HEIGHT);
    
    if (CurrentTile == NULL)
        return;
    
    if (CurrentTile != CMap::Map.MouseOverTile && Left)
    {        
        CMap::Map.MouseOverTile = CurrentTile;
        CMap::Map.DrawTile(CurrentTile, TileSelector.SelectedTile);
    }
}