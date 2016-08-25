#ifndef _GAME_MNG_H_
#define _GAME_MNG_H_

#include <map>
#include "Room.h"
#include "Common.h"

class GameMng
{
public:
    GameMng();
    int SitDown();
private:
    std::map<int, Room> rooms_; //<id, Room>
};

#endif
