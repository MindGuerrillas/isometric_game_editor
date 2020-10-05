
#include "CGame.h"
#include "CEventsManager.h"

void CGame::Loop() 
{    
    // Run any Managed Events
    CEventsManager::EventsManager.Loop();
  
    //CLevel::Level.GetCurrentArea()->Loop();
   
    // Give some CPU back
    SDL_Delay(1);
}


