#include "CMap.h"
#include "CSurfaceManager.h"
#include "CCamera.h"
#include "CTilesetManager.h"
#include "CApp.h"
#include <fstream>
#include <sstream>

CMap    CMap::Map;

CMap::CMap()
{
    AutoCoast = true;
    ShowGrid = true;
    MapHeight = 0;
    MapWidth = 0;   
    StartTile = NULL;
    EndTile = NULL;
    PathCalculated = false;
}

void CMap::Load()
{
    // Load in Tileset Graphics
    CTilesetManager::TilesetManager.GetSurface(TILESET_GFX);
    CTilesetManager::TilesetManager.GetSurface(OVERLAYS_GFX, false);
    CTilesetManager::TilesetManager.GetSurface(PATH_ARROWS, false);
    CTilesetManager::TilesetManager.GetSurface(TERRAIN_HILLS_GFX, false);
    CTilesetManager::TilesetManager.GetSurface(TERRAIN_MOUNTAINS_GFX, false);
    CTilesetManager::TilesetManager.GetSurface(TERRAIN_TREES_GFX, false);
    CTilesetManager::TilesetManager.GetSurface(TERRAIN_RIVERS_GFX, false);
    CTilesetManager::TilesetManager.GetSurface(RIVER_MOUTHS_GFX, false);
    
    // Read & Load Map file
    std::string line;
    std::ifstream  mapfile (Filename.c_str());
    
    if (!mapfile.is_open()) // can't open file so generate a new one
    {
        FILE* f = fopen("./maps/newmap.map", "w");

        if(f == NULL) return;

        // 50x80 map
        for (int Y = 0; Y < 80; Y++) 
        {
            for (int X = 0; X < 50; X++)          
            {                
                fprintf(f, "0:-1 ");            
            }

            fprintf(f, "\n");
        }

        fclose(f);
        
        // Load the new file;
        Filename = "./maps/newmap.map";
        
        Load();        
    }
    
    int TileCounter = 0;
    
    while(getline(mapfile, line))
    {
        // split line by ' ' SPACE
        std::stringstream ss(line);        
        std::string tiledata;
        
        while(getline(ss,tiledata, ' '))
        {
            // Split data by : into Tile Type & Special Terrain
            
            std::stringstream sstile(tiledata); 
            std::string tiletype;
            std::string tilespecialterrain;
            
            getline(sstile,tiletype,':');
            getline(sstile,tilespecialterrain,':');
            
            // Load Tile
            CTile tile;
            tile.TileType = atoi(tiletype.c_str());
            tile.SpecialTerrain = atoi(tilespecialterrain.c_str());
            tile.Seed = rand() % 3;
            tile.ID = TileCounter; 
            
            TileList.push_back(tile);
            
            TileCounter++;
            
            if (MapHeight == 0)
                MapWidth++;
        }
                
        MapHeight++;
    }
               
    // Calculate X & Y positions for Tiles
    for (int id=0, len=TileList.size(); id < len; id++)
    {
        int X = id % MapWidth;
        int Y = id / MapWidth;
        int XOffSet;
        
        // Offset row by TILE_WIDTH/2 on even rows        
        if (Y % 2 == 0) // it's EVEN       
            XOffSet = 0;                                  
        else // it's ODD        
            XOffSet = TILE_WIDTH / 2;
                
        int tileX = (X * TILE_WIDTH) + XOffSet;
        int tileY = Y * TILE_HEIGHT/2;
        
        TileList[id].X = tileX;
        TileList[id].Y = tileY;
        TileList[id].XGridPos = X;
        TileList[id].YGridPos = Y;
    }
        
    // Generate AutoCoast & Terrain Info for Tiles;
    for (int id=0, len=TileList.size(); id < len; id++)
    {
        GenerateAutoCoast(&TileList[id]);
        
        if ((TileList[id].TileType >= TILE_TYPE_FORESTS && TileList[id].TileType <= TILE_TYPE_MOUNTAINS) || TileList[id].SpecialTerrain == RIVER_TILE)
            GenerateAutoTerrain(&TileList[id]);
    }
    
    mapfile.close();    
}

void CMap::Render(SDL_Surface* SDisplay, int MapX, int MapY)
{  
    int ID = 0;

    for (int y = 0; y < MapHeight; y++)
    {
        int screen_tileY = MapY + TileList[ID].Y;

        for (int x = 0; x < MapWidth; x++)
        {                
            // Get Current Tile
            CTile* tile = GetTileByID(ID);
            
            if (PathCalculated) 
                tile->ClearPathInfo();
            
            int screen_tileX = MapX + tile->X;
            
            // get position in pixels of texture in the tileset image
            TilePosition* tPos = CTilesetManager::TilesetManager.GetTileByRowCol(TILESET_GFX, tile->Seed, tile->TileType);
            
            if (CCamera::Camera.OnScreen(tile->X, tile->Y))
            {
                // Draw tile to surface
                CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(TILESET_GFX), 
                                screen_tileX, screen_tileY, tPos->X, tPos->Y, TILE_WIDTH, TILE_HEIGHT);
                
                // Overlay Coast Map
                tile->RenderCoast(SMap);
                
                 // Show Grid
                if (ShowGrid)
                   CSurface::Draw(SMap, CSurfaceManager::SurfaceManager.GetSurface(GRID_GFX), screen_tileX, screen_tileY, 0, 0, TILE_WIDTH, TILE_HEIGHT); 
                
                // add any terrain - for Forests, Hills, Mountains
                if (tile->TileType >= TILE_TYPE_FORESTS && tile->TileType <= TILE_TYPE_MOUNTAINS)
                {
                    std::string tileset;
                    
                    switch (tile->TileType)
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
                    
                    TilePosition* tPosTerrain = CTilesetManager::TilesetManager.GetTile(tileset, tile->TileTerrainIndex);
                    
                    CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(tileset), 
                            screen_tileX, screen_tileY, tPosTerrain->X, tPosTerrain->Y, TILE_WIDTH, TILE_HEIGHT);                    
                }
                
                // add any special terrain
                if (tile->SpecialTerrain >= 0)
                {
                    if (tile->SpecialTerrain == RIVER_TILE)
                    {
                        TilePosition* tPosRiver = CTilesetManager::TilesetManager.GetTile(TERRAIN_RIVERS_GFX,tile->TileTerrainIndex);

                        CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(TERRAIN_RIVERS_GFX), 
                                screen_tileX, screen_tileY, tPosRiver->X, tPosRiver->Y, TILE_WIDTH, TILE_HEIGHT);                                            

                        if (tile->RiverMouth >= 0)
                        {
                            TilePosition* tPosRiver = CTilesetManager::TilesetManager.GetTile(RIVER_MOUTHS_GFX,tile->RiverMouth);

                            CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(RIVER_MOUTHS_GFX), 
                                screen_tileX, screen_tileY, tPosRiver->X, tPosRiver->Y, TILE_WIDTH, TILE_HEIGHT); 
                        }
                    } else
                    {
                        TilePosition* tPosSpecialTerrain = CTilesetManager::TilesetManager.GetTile(OVERLAYS_GFX,tile->SpecialTerrain);

                        CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(OVERLAYS_GFX), 
                                screen_tileX, screen_tileY, tPosSpecialTerrain->X, tPosSpecialTerrain->Y, TILE_WIDTH, TILE_HEIGHT);
                    }
                }
                
                // add an improvement
                if (tile->TileImprovement >= 0)
                {
                    TilePosition* tPosImprovements = CTilesetManager::TilesetManager.GetTile(OVERLAYS_GFX,tile->TileImprovement);
                    
                    CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(OVERLAYS_GFX), 
                            screen_tileX, screen_tileY, tPosImprovements->X, tPosImprovements->Y, TILE_WIDTH, TILE_HEIGHT);
                }
                
                // Show Path Arrow if Needed;
                if (tile->ShowArrow >= 0)
                {
                    TilePosition* tPosArrow = CTilesetManager::TilesetManager.GetTile(PATH_ARROWS,tile->ShowArrow);
                    
                    CSurface::Draw(SMap, CTilesetManager::TilesetManager.GetSurface(PATH_ARROWS), 
                            screen_tileX, screen_tileY, tPosArrow->X, tPosArrow->Y, TILE_WIDTH, TILE_HEIGHT);                    
                }
                
            }
            
            ID++;
        }
    }
    
    PathCalculated = false;
    
    CSurface::Draw(SDisplay, SMap, 0, 32);
}

int CMap::Height()
{
    return MapHeight;
}

int CMap::Width()
{
    return MapWidth;
}

void CMap::Tidy()
{
    TileList.clear();
    CTilesetManager::TilesetManager.Tidy();
}

int CMap::GetTileIDFromGridPos(int X, int Y)
{
    return (MapWidth * Y) + X;
}

CTile* CMap::GetTileByCoord(int X, int Y) 
{
    SDL_Surface* MouseMap = CSurfaceManager::SurfaceManager.GetSurface(MOUSEMAP_GFX);
    
    static bool ColoursLoaded = false;
    
    if (!ColoursLoaded)
    {
        red = SDL_MapRGB(MouseMap->format, 255, 0, 0);
        yellow = SDL_MapRGB(MouseMap->format, 255, 255, 0);
        white = SDL_MapRGB(MouseMap->format, 255, 255, 255);
        green = SDL_MapRGB(MouseMap->format, 0, 255, 0);
        blue = SDL_MapRGB(MouseMap->format, 0, 0, 255);
        ColoursLoaded = true;
    }
    
    int MapX = X + CCamera::Camera.GetX();
    int MapY = Y + CCamera::Camera.GetY();;
    
    int RegionX = MapX / TILE_WIDTH;
    int RegionY = (MapY / TILE_HEIGHT) * 2;    
    
    int MouseMapX = MapX % TILE_WIDTH;
    int MouseMapY = MapY % TILE_HEIGHT;
 
    // Get Colour in MouseMap @ MouseMapX,MouseMapY
    int PixelColour = CSurface::getpixel(MouseMap, MouseMapX, MouseMapY);

    int RegionDX = 0;
    int RegionDY = 0;
 
    if (PixelColour == red)
    {
        RegionDX = -1;
        RegionDY = -1;
    }else if (PixelColour == yellow)
    {
        RegionDX = 0;
        RegionDY = -1;
    }else if (PixelColour == white)
    {
        RegionDX = 0;
        RegionDY = 0;
    }else if (PixelColour == green)
    {
        RegionDX = -1;
        RegionDY = 1;
    }else if (PixelColour == blue)
    {
        RegionDX = 0;
        RegionDY = 1;
    }
    
    unsigned int ID = (MapWidth * (RegionY + RegionDY)) + (RegionX + RegionDX);
    
    if(ID < 0 || ID >= TileList.size())
    	return NULL;
    
    return &TileList[ID];
}

CTile* CMap::GetTileByGridPos(int X, int Y)
{
    int ID = GetTileIDFromGridPos(X, Y);
    
    return &TileList[ID];
}

CTile* CMap::GetTileByID(int ID)
{
    if (ID < 0 || ID > GetNumberOfTiles())
        return NULL;
    
    return &TileList[ID];
}

int CMap::GetDirectionToTile(CTile* From, CTile* To)
{        
    for (int x = 0; x < NUMBER_OF_DIRECTIONS; x++)
        if (GetTileByDirection(From, x) == To)
            return x;
    
    return -1;
}

CTile* CMap::GetTileByDirection(CTile* Tile, int Direction)
{
    int TileY, TileX;
    
    switch(Direction)
    {
        case NORTH:
            TileX = Tile->XGridPos;
            TileY = Tile->YGridPos - 2;
            break;
            
        case NORTH_EAST: // if EVEN
            TileX = Tile->XGridPos + ((Tile->YGridPos % 2 == 0) ? 0 : 1);
            TileY = Tile->YGridPos - 1;
            break;
            
        case EAST:
            TileX = Tile->XGridPos + 1;
            TileY = Tile->YGridPos;
            break;
            
        case SOUTH_EAST: // if EVEN
            TileX = Tile->XGridPos + ((Tile->YGridPos % 2 == 0) ? 0 : 1);
            TileY = Tile->YGridPos + 1;
            break;
            
        case SOUTH:
            TileX = Tile->XGridPos;
            TileY = Tile->YGridPos + 2;
            break;
            
        case SOUTH_WEST: // if EVEN
            TileX = Tile->XGridPos + ((Tile->YGridPos % 2 == 0) ? -1 : 0);
            TileY = Tile->YGridPos + 1;
            break;

        case WEST:
            TileX = Tile->XGridPos - 1;
            TileY = Tile->YGridPos;
            break;
            
        case NORTH_WEST: // if EVEN
            TileX = Tile->XGridPos + ((Tile->YGridPos % 2 == 0) ? -1 : 0);
            TileY = Tile->YGridPos - 1;
            break;                   
    }
    
    if (TileX < 0 || TileY < 0)
        return NULL;
    
    return GetTileByGridPos(TileX, TileY);
}

void CMap::GenerateAutoCoast(CTile* Tile)
{
    if (Tile == NULL)
        return;
    
    // Is this Tile an Ocean Tile. If so find if it is next to land. If so, in which direction   
    if (Tile->TileType != TILE_TYPE_WATER)
        return;
    
    // Split Tile into 4 (N,S,E,W) & Check each for Land
    
    char PatternMap[3] = {'\0'};
    int  Counter;
    bool LandFound = false;
        
    for (int direction = 0; direction < NUMBER_OF_DIRECTIONS; direction +=2)
    {
        Counter = 0;
        
        for (int d2 = (direction - 1); d2 <= (direction + 1); d2++)
        {
            int d3 = (d2 < 0) ? NUMBER_OF_DIRECTIONS - 1: d2;
            
            CTile* nextTile = GetTileByDirection(Tile, d3);
            
            if (nextTile)            
            {
                if (nextTile->TileType != TILE_TYPE_WATER)
                {
                    PatternMap[Counter] = 'L'; // Land is here
                    LandFound = true;
                }else
                    PatternMap[Counter] = 'W';
            } else
                PatternMap[Counter] = 'X';
                
            Counter++;
        }  

        // Terminate string;
        PatternMap[3] = '\0';
    
        Tile->CoastMap[direction / 2] = GetCoastTilePos(PatternMap);
    }
    
    Tile->HasCoast = LandFound;    
}

int CMap::GetCoastTilePos(char* pattern)
{
    if (strcmp(pattern,"WWW") == 0)
        return 0;

    if (strcmp(pattern,"LWW") == 0)
        return 1;

    if (strcmp(pattern,"WLW") == 0)
        return 2;

    if (strcmp(pattern,"LLW") == 0)
        return 3;

    if (strcmp(pattern,"WWL") == 0)
        return 4;

    if (strcmp(pattern,"LWL") == 0)
        return 5;

    if (strcmp(pattern,"WLL") == 0)
        return 6;

    if (strcmp(pattern,"LLL") == 0)
        return 7;
    
    return -1;
}

int CMap::GetNumberOfTiles()
{
    return MapWidth * MapHeight;
}

void CMap::DisplayRoute()
{
    
}

void CMap::ClearPathArrows()
{
    for (int x=0, len = TileList.size(); x < len; x++)
        TileList[x].ShowArrow = -1;
}

void CMap::DrawTile(CTile* Tile, int ID)
{
    if (ID == 11) // RIVER
    {
        if ((Tile->TileType != TILE_TYPE_WATER) && (Tile->TileType != TILE_TYPE_MOUNTAINS) && (Tile->TileType != TILE_TYPE_HILLS))
            Tile->SpecialTerrain = RIVER_TILE;
    }else
        Tile->TileType = ID;
   
    // Clear CoastMap
    Tile->HasCoast = false;
    
    // Make New Coastline    
    // For This Tile
    GenerateAutoCoast(Tile);
    GenerateAutoTerrain(Tile);
    
    // For Surrounding Tiles
    for (int x = 0; x < NUMBER_OF_DIRECTIONS; x++)
    {
        CTile* nextTile = GetTileByDirection(Tile, x);        
        if (nextTile)
        {
            GenerateAutoCoast(nextTile);
            GenerateAutoTerrain(nextTile);
        }
    }
}

void CMap::Save()
{
    FILE* f = fopen(Filename.c_str(), "w");

    if(f == NULL) return;

    // 50x80 map
    for (int Y = 0; Y < MapHeight; Y++) 
    {
       for (int X = 0; X < MapWidth; X++)          
       {
           int ID = X + (MapWidth * Y);
           fprintf(f, "%d:%d ", TileList[ID].TileType, TileList[ID].SpecialTerrain);            
       }

       fprintf(f, "\n");
    }

    fclose(f);   
    
}

void CMap::GenerateAutoTerrain(CTile* Tile)
{
    // Check Diagonal Tiles for Tiles of same terrain Type. 
    // Calculate a number from 0 -> 15
    
    int ID = 0;
    int counter = 0;
    int isRiverMouth = -1;
    
    for (int x = 1; x < NUMBER_OF_DIRECTIONS; x += 2)
    {
        CTile* checkTile = GetTileByDirection(Tile,x);
        
        if (Tile != NULL) // No Tile Available so set to 0 (ignore it);  
        {
            // RIVERS
            if (Tile->SpecialTerrain == RIVER_TILE)
            {                
                if (Tile->SpecialTerrain == checkTile->SpecialTerrain)
                    ID |= 1 << counter;
                else if (checkTile->TileType == TILE_TYPE_WATER)
                    isRiverMouth = counter;                
            }else if (Tile->TileType == checkTile->TileType)
                ID |= 1 << counter;
        }
        
        counter++;
    }
    
    if (isRiverMouth >= 0)  
        Tile->RiverMouth = isRiverMouth;        
    
    Tile->TileTerrainIndex = ID;
}