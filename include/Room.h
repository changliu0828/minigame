#ifndef _ROOM_H_
#define _ROOM_H_

#include <vector>
#include "Common.h"
#include "Player.h"
class Room 
{
public:
    Room();
    void init();

private:
    int id_;
    E_ROOM_STATUS status_;
    std::vector<Player> players_;   
};


#endif
