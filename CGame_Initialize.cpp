#include "CGame.h"
#include "CDebugLogging.h"

#include <string>
#include <algorithm>
#include "CFramerate.h"
#include "CMap.h"

bool CGame::Activate(SDL_Surface* SDisp, int Options[]) 
{    
    SDisplay = SDisp;
    
    // Load Game
    CMap::Map.Load();
    
    // Reset the Framerate to avoid big difference in FPS between States
    CFramerate::FPSControl.Reset();
    
    TileSelector.X = 0;
    TileSelector.Y = 0;        
    TileSelector.Load(SDisplay);
    CControl::ControlList.push_back(&TileSelector);
    
    return true;
}

void CGame::DeActivate()
{
    Tidy();
}

