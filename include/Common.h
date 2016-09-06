#ifndef _COMMON_H_
#define _COMMON_H_

#include <muduo/base/Logging.h>

#define CLIENT_FREQ  30 // client update frequence
#define MAX_ROOM_NUM 10 // max room nubmers
#define GAME_TIME 5     //5 minute
#define PLAYER_NUM_PER_ROOM 2 
enum E_MSG_TYPE
{
    MSG_MATCH_REQ = 0,  //      {"MSG_TYPE":0, "NAME":""}
    MSG_MATCH_RSP = 1,  //      {"MSG_TYPE":1, "RET":0, "VALUE":[bornX, bornY])}
                        //      {"MSG_TYPE":1, "RET":1, "REASON":E_REASON)}                              
    MSG_FRAME_REQ = 2,  //      {"MSG_TYPE":2, "FRAME_SEQ":0...GAME_TIME*60*CLIENT_FREQ, "ACT":""}
    MSG_FRAME_RSP = 3,  //      {"MSG_TYPE":3, "FRAME_SEQ":0...GAME_TIME*60*CLIENT_FREQ, "RET":0, "RESULT":TODO-2POS, PLAYER_STAT, OTHER }
                        //      {"MSG_TYPE":3, "FRAME_SEQ":0...GAME_TIME*60*CLIENT_FREQ, "RET":1, "REASON":E_REASON }
};

enum E_ACT
{
    ACT_MOVE_UP     = 0,
    ACT_MOVE_LEFT   = 1,
    ACT_MOVE_DOWN   = 2,
    ACT_MOVE_RIGHT  = 3,
    ACT_TRIGGER     = 4
};

enum E_REASON
{
    REASON_NO_EMPTY_ROOM     = 0,
    REASON_GAME_OVER_DIE     = 1,
    REASON_GAME_OVER_TIMEOUT = 2,
    REASON_GAME_WIN          = 3,
    REASON_CONNETCTION_LOST_MY = 4,
    REASON_CONNETCTION_LOST_OTH = 5
};

enum E_ROOM_STATUS
{
    ROOM_EMPTY = 0, //empty room
    ROOM_WAIT  = 1, //have one player
    ROOM_START = 2  //already start
};


#endif
