#include "CGame.h"
#include <string>
#include "CDebugLogging.h"
#include "CStateManager.h"
#include "CFramerate.h"
#include "CMap.h"

CGame CGame::Instance;

CGame::CGame()
{
    CurrentLevel = 0;   
}

void CGame::Render() 
{
    if (CFramerate::FPSControl.Framerate_Control())
    {  
        SDL_Rect Rect;
        Rect.x = 0;
        Rect.y = 0;
        Rect.w = WINDOW_WIDTH;
        Rect.h = WINDOW_HEIGHT;

        // Print map to display
        // Clear display
        SDL_FillRect(SDisplay, &Rect, 0x000000);

        CMap::Map.Render(SDisplay,-CCamera::Camera.GetX(), -CCamera::Camera.GetY());

            // Render Controls
        for (int  i=0; i < CControl::ControlList.size();i++)
                CControl::ControlList[i]->Render(SDisplay);
        
        //Flip to screen
        SDL_Flip(SDisplay);
    }
}

void CGame::Tidy() 
{
    // Logging
    CDebugLogging::DebugLogging.Log("Game Closing - Tidying", 1); 

    CurrentLevel = 0;

    CMap::Map.Tidy();
    
    // Close Debug Log if open
    CDebugLogging::DebugLogging.Tidy();
}

void CGame::Reset()
{    
    //InitializeEntities();
    
    CSoundManager::SoundManager.StopMusic();
    
    CDebugLogging::DebugLogging.Log("Game Reset", 1);    
}

CGame* CGame::GetInstance()
{
    return &Instance;
}

void CGame::GameOver()
{    
    CSoundManager::SoundManager.Play(FX_GAME_OVER);
    
    // Switch States and Show Level Transition Screen
    CStateManager::SetAppState(STATE_GAME_OVER, SDisplay);    
}

void CGame::GameComplete()
{    
    // Switch States and Show Level Transition Screen
    CStateManager::SetAppState(STATE_GAME_COMPLETE, SDisplay);    
}