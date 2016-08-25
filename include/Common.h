#ifndef _COMMON_H_
#define _COMMON_H_

#define NOTIFY_INTERVAL 1 //time interval of updating to client
#define MAX_ROOM_NUM 10 //max room nubmers


enum E_MSG_ID
{
    MSG_HELLO = 0,
    MSG_MATCH = 1,
    MAS_ACT =   2
};

enum E_ROOM_STATUS
{
    ROOM_EMPTY = 0, //empty room
    ROOM_WAIT  = 1, //have one player
    ROOM_START = 2  //already start
};


#endif
