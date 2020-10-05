/* 
 * File:   CTile.h
 * Author: alan
 *
 * Created on March 4, 2013, 2:55 PM
 */

#ifndef CTILE_H
#define	CTILE_H

#include <SDL.h>

#define TILE_WIDTH      64
#define TILE_HEIGHT     32

#define COAST_GFX       "./gfx/tilesets/coast.png"
#define RIVER_TILE      999
#define RIVER_MOUTH     998

enum
{
    TILE_TYPE_WATER,            // 0
    TILE_TYPE_GRASS,            // 1
    TILE_TYPE_DESERT,           // 2
    TILE_TYPE_PRAIRI,           // 3
    TILE_TYPE_FORESTS,
    TILE_TYPE_HILLS,
    TILE_TYPE_MOUNTAINS,
    TILE_TYPE_TUNDRA,           // 7
    TILE_TYPE_ARTIC,
    TILE_TYPE_SWAMP,
    TILE_TYPE_JUNGLE            // 10
};

enum
{
    TILE_IMPROVEMENT_IRRIGATED,
    TILE_IMPROVEMENT_FARM,
    TILE_IMPROVEMENT_MINE
};

enum
{
    TILE_TERRAIN_NORMAL,
    TILE_TERRAIN_MOUNTAIN,
    TILE_TERRAIN_RIVER
};

struct PathFindingInfo
{
    int ParentID;
    int G;
    int H;
    int F;
    int WhichList;
};

class CTile
{
    public:
        
        CTile();
    
        int     X;
        int     Y;
        int     XGridPos;
        int     YGridPos;
        int     ID; 
        
        int     TileType; // Grass, Plains, Hill, Mountain etc..
        int     SpecialTerrain; // furs, buffalo, corn, gold, etc
        
        int     TileImprovement; // Farm, mine, irrigation etc
        
        int     TileTerrainIndex;  // Stores index of image to display for tile
        int     RiverMouth; // End of river. Number is for which direction mouth goes in;
        
        int     ShowArrow;
        
        int     Seed; // Random value to smooth out tiled effect
        bool    HasCoast;         
        int     CoastMap[4];    
        
        // Path Finding Information
        PathFindingInfo PathInfo;   
        
        int     TerrainCost();
        
        void    RenderCoast(SDL_Surface* SDisplay);
        bool    isPassable();
        void    ClearPathInfo();
};


#endif	/* CTILE_H */

