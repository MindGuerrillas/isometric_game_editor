/* 
 * File:   CControl.h
 * Author: alan
 *
 * Created on December 6, 2012, 9:56 AM
 */

#ifndef CCONTROL_H
#define	CCONTROL_H

#include <vector>
#include "CSurface.h"
#include "CEvent.h"

class CControl : public CEvent
{
    public:
        static std::vector<CControl*>   ControlList;
        
        char*           tag;
        int             X, Y, W, H;
        
        CControl();        
        virtual ~CControl();
        
        virtual bool Load(SDL_Surface* SDisplay);
        virtual void Render(SDL_Surface* SDisplay);
        virtual void Tidy();
        
        virtual void OnClick(int x, int y);
        
        void OnLButtonUp(int x, int y);
        
    protected:
        SDL_Surface*    SControlSurface;
        
    private:
        
    
};


#endif	/* CCONTROL_H */

