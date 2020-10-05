
#include "CCamera.h"
#include "CMap.h"

CCamera CCamera::Camera;

CCamera::CCamera() 
{
    X = TILE_WIDTH / 2;
    Y = TILE_HEIGHT / 2;

    TargetX = NULL;
    TargetY = NULL;

    TargetMode = TARGET_MODE_NORMAL;
}

// Moves camera by MoveX & MoveY
void CCamera::OnMove(int MoveX, int MoveY) 
{
    int MapWidthPixels = CMap::Map.Width() * TILE_WIDTH;
    int MapHeightPixels = CMap::Map.Height() / 2 * TILE_HEIGHT;
    
    // don't move for small maps
    if ((MapWidthPixels < WINDOW_WIDTH) || (MapHeightPixels < WINDOW_HEIGHT))
        return;
    
    X += MoveX;
    Y += MoveY;

    // Prevent seeing off screen
    if (X < TILE_WIDTH / 2) X = TILE_WIDTH / 2;
    if (Y < TILE_HEIGHT / 2) Y = TILE_HEIGHT / 2;
    if (X > (MapWidthPixels - WINDOW_WIDTH)) X = MapWidthPixels - WINDOW_WIDTH;
    if (Y > (MapHeightPixels - WINDOW_HEIGHT)) Y =  MapHeightPixels - WINDOW_HEIGHT;
        
}

int CCamera::GetX() 
{
    return X;
}


int CCamera::GetY() 
{
    return Y;
}


void CCamera::SetPos(int X, int Y) 
{
    this->X = X;
    this->Y = Y;
}


void CCamera::SetTarget(float* X, float* Y) 
{
    TargetX = X;
    TargetY = Y;
}

bool CCamera::OnScreen(int objX, int objY)
{
    bool WithinX = false;
    bool WithinY = false;
    
    if (objX > (X - TILE_WIDTH) && objX < X + WINDOW_WIDTH + TILE_WIDTH)
        WithinX = true;
    
    if (objY > (Y - TILE_HEIGHT) && objY < Y + WINDOW_HEIGHT + TILE_HEIGHT)
        WithinY = true;
    
    return (WithinX && WithinY);     
}