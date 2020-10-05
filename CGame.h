#ifndef _CGAME_H_
    #define _CGAME_H_

#include <vector>
#include <string>

#include "CState.h"
#include "Define.h"
#include "CSoundManager.h"
#include "CCamera.h"
#include "CControl.h"
#include "CTileset.h"

class CGame : public CState 
{
    private:
        
        static CGame    Instance;
        CGame();
        SDL_Surface*    SDisplay;
        
        int                             CurrentLevel; // Index for Levels list
        std::vector<std::string>        Levels; // List of levels
        
    public:
        void Reset(); 
        
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
        void OnUser(Uint8 type, int code, void* data1, void* data2);             
        void OnLButtonDown(int x, int y);
        void OnRButtonDown(int x, int y);
        void OnMouseMove(int mX, int mY, int relX, int relY, bool Left,bool Right,bool Middle);
        
        bool Activate(SDL_Surface* SDisp, int Options[] = NULL);
        void DeActivate();
        void Loop();
        void Render();
        void Tidy();        
        
        void    GameOver();
        void    GameComplete();
        
        static CGame* GetInstance();
        
        int     BinaryTest[10];
        int     SizeofArray;
        
        CTileset        TileSelector;
       
};

#endif
