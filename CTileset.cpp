#include "CMap.h"
#include "CTileset.h"
#include <string>
#include "CGame.h"
#include "CSurface.h"
#include "CTilesetManager.h"

CTileset::CTileset() 
{
    STileset = NULL;
    STile_Grid = NULL;
    STile_Highlight = NULL;
    SelectedTile = 0;
}

bool CTileset::Load(SDL_Surface* SDisplay) 
{   
    // Load Tile set
    STileset = CTilesetManager::TilesetManager.GetSurface(TILESET_GFX);
    
    W = 768;
    H = 32;
    
    CControl::Load(SDisplay);
}

void CTileset::Render(SDL_Surface* SDisplay) 
{  
    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.w = W;
    Rect.h = H;
    
    SDL_FillRect(SControlSurface, &Rect, 0xdedede);
    
    // Draw to Control Surface 0 -> 10 Normal Terrain; 11 Rivers
    for (int x = 0; x <= 11; x++)
    {
        if (SelectedTile == x)
        {
            // Highlight tile
            SDL_Rect Rect;
            Rect.x = x * TILE_WIDTH;
            Rect.y = 0;
            Rect.w = TILE_WIDTH;
            Rect.h = TILE_HEIGHT;

            SDL_FillRect(SControlSurface, &Rect, 0xff0000);            
        }
        
        if (x == 11) // River
        {
            TilePosition* tp = CTilesetManager::TilesetManager.GetTileByRowCol(TERRAIN_RIVERS_GFX, 2, 1);

            CSurface::Draw(SControlSurface, CTilesetManager::TilesetManager.GetSurface(TERRAIN_RIVERS_GFX), 
                    x * TILE_WIDTH, 0, tp->X, tp->Y, TILE_WIDTH, TILE_HEIGHT);            
        }
        else
        {
            TilePosition* tp = CTilesetManager::TilesetManager.GetTileByRowCol(TILESET_GFX, 0, x);

            CSurface::Draw(SControlSurface, CTilesetManager::TilesetManager.GetSurface(TILESET_GFX), 
                    x * TILE_WIDTH, 0, tp->X, tp->Y, TILE_WIDTH, TILE_HEIGHT);

            // add any terrain - for Forests, Hills, Mountains
            if (x >= TILE_TYPE_FORESTS && x <= TILE_TYPE_MOUNTAINS)
            {
                std::string tileset;

                switch (x)
                {
                    case TILE_TYPE_MOUNTAINS:
                        tileset = TERRAIN_MOUNTAINS_GFX;
                        break;

                    case TILE_TYPE_HILLS:
                        tileset = TERRAIN_HILLS_GFX;
                        break;

                    case TILE_TYPE_FORESTS:
                        tileset = TERRAIN_TREES_GFX;
                        break;
                }

                TilePosition* tPosTerrain = CTilesetManager::TilesetManager.GetTile(tileset, 0);

                CSurface::Draw(SControlSurface, CTilesetManager::TilesetManager.GetSurface(tileset), 
                        x * TILE_WIDTH, 0, tPosTerrain->X, tPosTerrain->Y, TILE_WIDTH, TILE_HEIGHT);                    
            }  
        }
    }
    
    CControl::Render(SDisplay);
}

void CTileset::Tidy() 
{
    if(STileset)
        SDL_FreeSurface(STileset);
    
    if(STile_Grid)
        SDL_FreeSurface(STile_Grid);

    if (STile_Highlight)
        SDL_FreeSurface(STile_Highlight);
    
    CControl::Tidy();
}

void CTileset::OnClick(int x, int y)
{
    // Control clicked - Do Something
    // Find out which tile was clicked
    
    int ID = x / TILE_WIDTH;
    
    SelectedTile = ID;    
}