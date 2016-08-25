#include "GameMng.h"

GameMng::GameMng()
{
    for (int i = 0; i < MAX_ROOM_NUM; ++ i) {
        Room newRoom;
        std::pair<int, Room> newPair(i, newRoom);
        rooms_.insert(newPair);  
    } 
}
