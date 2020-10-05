#include <string>
#include "CTile.h"
#include "CTilesetManager.h"
#include "CDebugLogging.h"
#include "CSurfaceManager.h"

CTilesetManager   CTilesetManager::TilesetManager;

CTilesetManager::CTilesetManager()
{

}

bool CTilesetManager::Initialize()
{   
    
    return true;
}

// Load a specified Surface
bool CTilesetManager::Load(std::string File, bool alpha)
{    
    // Load Tileset image
    Tileset set;
    set.Surface = CSurfaceManager::SurfaceManager.GetSurface(File, alpha);
    
    // Load in the details of the tileset's tiles
    int TilesetWidth  = set.Surface->w / TILE_WIDTH;
    int TilesetHeight = set.Surface->h / TILE_HEIGHT;
    int ID = 0;
    
    set.Width = TilesetWidth;
    set.Height = TilesetHeight;
    
    for (int y=0; y < TilesetHeight; y++)
    {
        for (int x=0; x < TilesetWidth; x++)
        {
            int TilesetX = (ID % TilesetWidth) * TILE_WIDTH;
            int TilesetY = (ID / TilesetWidth) * TILE_HEIGHT;
            
            TilePosition tPos;
            tPos.X = TilesetX;
            tPos.Y = TilesetY;
            
            set.Tiles.push_back(tPos);
            
            ID++;
        }
    }
    
    Tilesets[File] = set;
    
    return true;
}

void CTilesetManager::Tidy()
{
    Tilesets.clear();
}

SDL_Surface* CTilesetManager::GetSurface(std::string ID, bool alpha)
{    
    // Check is requested surface is already loaded. If not, do so
    if (Tilesets[ID].Surface == NULL)
        if (Load(ID, alpha) == false) return NULL;
       
    return Tilesets[ID].Surface;
}

TilePosition* CTilesetManager::GetTile(std::string ID, int tileID)
{
    return &Tilesets[ID].Tiles[tileID];
}

TilePosition* CTilesetManager::GetTileByRowCol(std::string ID, int X, int Y)
{
    if (X >= Tilesets[ID].Width) X = Tilesets[ID].Width;
    if (Y >= Tilesets[ID].Height) Y = Tilesets[ID].Height;
    
    int tileID = (Y * Tilesets[ID].Width) + X;
    
    return &Tilesets[ID].Tiles[tileID];
}