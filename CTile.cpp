#include "CTile.h"
#include "CMap.h"
#include "CSurfaceManager.h"
#include "CCamera.h"

CTile::CTile()
{
    ShowArrow = -1;
    TileImprovement = -1;
    TileTerrainIndex = 0;
    RiverMouth = -1;
    
    HasCoast = false;
    
    PathInfo.F = 0;
    PathInfo.G  = 0;
    PathInfo.H  = 0;
    PathInfo.ParentID = -1;
    PathInfo.WhichList = 0;
    
    // Clear PatternMap array
    for (int x=0; x < 4; x++)
        CoastMap[x] = -1;
}

void CTile::RenderCoast(SDL_Surface* SDisplay)
{
    if (HasCoast && CMap::Map.AutoCoast)
    {
        SDL_Surface* coast = CSurfaceManager::SurfaceManager.GetSurface(COAST_GFX);

        int EvenX = -CCamera::Camera.GetX() + (X + (TILE_WIDTH / 4));
        int EvenY = -CCamera::Camera.GetY() + (Y);
        
        int OddX = -CCamera::Camera.GetX() + X + (TILE_WIDTH / 2);
        int OddY = -CCamera::Camera.GetY() + (Y + (TILE_HEIGHT / 4));

        for (int c=0; c < 4; c++)
        { 
            if (c % 2 == 0) // EVEN TILES (N & S)
            {
                if (CoastMap[c] >= 0)
                    CSurface::Draw(SDisplay, coast, EvenX, EvenY, CoastMap[c] * (TILE_WIDTH/2), c * (TILE_HEIGHT/2), TILE_WIDTH/2, TILE_HEIGHT/2);
                
                EvenY += (TILE_HEIGHT / 2);
            }else // ODD TILE (W & E)
            {
                if (CoastMap[c] >= 0)
                    CSurface::Draw(SDisplay, coast, OddX, OddY, CoastMap[c] * (TILE_WIDTH/2), c * (TILE_HEIGHT/2), TILE_WIDTH/2, TILE_HEIGHT/2);

                OddX -= (TILE_WIDTH / 2);                            
            }                        
        }

    }    
}

bool CTile::isPassable()
{
    bool Passable = true;
    
    if (TileType == TILE_TYPE_WATER)
        Passable = false;
    
    if (TileType == TILE_TYPE_MOUNTAINS)
        Passable = false;
    
    return Passable;
}

void CTile::ClearPathInfo()
{
    PathInfo.F = 0;
    PathInfo.G = 0;
    PathInfo.H = 0;
    PathInfo.ParentID = -1;
    PathInfo.WhichList = 0;
}

int CTile::TerrainCost()
{
    switch (TileType)
    {
        case TILE_TYPE_FORESTS:
            return 5;
        case TILE_TYPE_HILLS:
            return 15;
        case TILE_TYPE_JUNGLE:
            return 10;
        case TILE_TYPE_SWAMP:
            return 10;            
    }
    
    return 0;
}