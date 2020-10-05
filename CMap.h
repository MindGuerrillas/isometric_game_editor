/* 
 * File:   CMap.h
 * Author: alan
 *
 * Created on March 4, 2013, 2:51 PM
 */

#ifndef CMAP_H
#define	CMAP_H

#include "CTile.h"
#include <vector>
#include <string>

enum {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
};

#define NUMBER_OF_DIRECTIONS    8
#define TILESET_GFX             "./gfx/tilesets/tiles.png"
#define OVERLAYS_GFX            "./gfx/tilesets/overlays.png"
#define MOUSEMAP_GFX            "./gfx/tilesets/mousemap.gif"
#define GRID_GFX                "./gfx/tilesets/grid.png"
#define PATH_ARROWS             "./gfx/tilesets/patharrows.png"
#define TERRAIN_TREES_GFX       "./gfx/tilesets/trees.png"
#define TERRAIN_HILLS_GFX       "./gfx/tilesets/hills.png"
#define TERRAIN_MOUNTAINS_GFX   "./gfx/tilesets/mountains.png"
#define TERRAIN_RIVERS_GFX      "./gfx/tilesets/rivers.png"
#define RIVER_MOUTHS_GFX        "./gfx/tilesets/rivermouths.png"

#define NUMBER_TERRAIN_TILES    16

class CMap
{
    public:
        CMap();
        
        std::string     Filename;
        
        static  CMap    Map;
        
        void    Render(SDL_Surface* SDisplay, int MapX, int MapY);
        
        void    Load();
        int     Width();
        int     Height();
        void    Tidy();
        
        CTile*  GetTileByCoord(int X, int Y) ;
        CTile*  GetTileByGridPos(int X, int Y);
        CTile*  GetTileByDirection(CTile* Tile, int Direction);
        CTile*  GetTileByID(int ID);
        int     GetTileIDFromGridPos(int X, int Y);
        
        int     GetDirectionToTile(CTile* From , CTile* To);
        
        int     GetNumberOfTiles();
        
        void    ClearPathArrows();
        
        bool    PathCalculated;
        bool    ShowGrid;
        bool    AutoCoast;
        
        CTile*  StartTile;
        CTile*  EndTile;
        CTile*  MouseOverTile;
        
        void    DisplayRoute();
        
        SDL_Surface*    SMap;
        
        void DrawTile(CTile* Tile, int ID);
        void Save();
        
    private:
        std::vector<CTile>      TileList;
        
        int     MapHeight;
        int     MapWidth;
        
        int     red, green, blue, yellow, white;
        
        void    GenerateAutoCoast(CTile* Tile); 
        
        void    GenerateAutoTerrain(CTile* Tile); // Calculates & sets Tile.TileTerrainIndex
        
        int     GetCoastTilePos(char* pattern);
};

#endif	/* CMAP_H */

