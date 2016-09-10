#ifndef _COMMON_H_
#define _COMMON_H_

#include <muduo/base/Logging.h>

#define CLIENT_FREQ  30 // client update frequence
#define MAX_ROOM_NUM 10 // max room nubmers
#define GAME_TIME 5     //5 minute
#define PLAYER_NUM_PER_ROOM 2 

enum E_MSG_TYPE
{
    MSG_SITDOWN_REQ = 0, 
    MSG_SITDOWN_RSP = 1,
    MSG_FRAME_REQ   = 2, 
    MSG_FRAME_RSP   = 3,

    MSG_EVENT_RSP   = 5,
};

enum E_EVENT_TYPE
{
    EVENT_SUCCESS           = 0,
    EVENT_NO_EMPTY_ROOM     = 1,
    EVENT_PARTNER_QUIT      = 2,
};

enum E_ROOM_STATUS
{
    ROOM_EMPTY = 0, //empty room
    ROOM_WAIT  = 1, //have one player
    ROOM_START = 2,  //already start
};


#endif
