/* 
 * File:   CTilesetManager.h
 * Author: alan
 *
 * Created on March 5, 2013, 1:20 PM
 */

#ifndef CTILESETMANAGER_H
#define	CTILESETMANAGER_H

#include <SDL.h>
#include "CSurface.h"
#include <tr1/unordered_map>
#include <vector>

struct TilePosition
{
    int X;
    int Y;
};

struct Tileset
{
    SDL_Surface*                Surface;
    std::vector<TilePosition>   Tiles;
    int                         Width;
    int                         Height;
};

typedef std::tr1::unordered_map  <std::string, Tileset> TilesetMap;

class CTilesetManager
{
    public:
        CTilesetManager();
        
        static CTilesetManager    TilesetManager;
        
        bool            Initialize();            
        void            Tidy();
        SDL_Surface*    GetSurface(std::string ID, bool alpha = true);
        
        TilePosition*   GetTile(std::string ID, int tileID);
        TilePosition*   GetTileByRowCol(std::string ID, int X, int Y);
        
    private:
        bool            Load(std::string File, bool alpha);        
                
        TilesetMap      Tilesets;    
};

#endif	/* CTILESETMANAGER_H */

