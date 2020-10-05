#ifndef _DEFINE_H_
    #define _DEFINE_H_

#define WINDOW_WIDTH    1024
#define	WINDOW_HEIGHT	768

#define UNINITIALIZED_PARAM     -999

enum {
    GAME_EVENT_RESET = 0,
    GAME_EVENT_GAMEOVER,
    GAME_EVENT_PAUSE,
    GAME_EVENT_LEVEL_COMPLETE
};

#define COLOR_RED       {221,58,25}
#define COLOR_WHITE     {255,255,255}
#define COLOR_YELLOW    {252,219,116}

#define LOGFILENAME     "isometric.log"

#endif
