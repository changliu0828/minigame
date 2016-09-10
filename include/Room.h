#ifndef _ROOM_H_
#define _ROOM_H_

#include <vector>
#include "Common.h"
#include "Player.h"
class Room 
{
public:
    Room(int id);
    void init();
    void addPlayer(Player player);
    void erasePlayer(string name);
    void eraseAllPlayer();
    Player& getAnotherPlayer(string myName);
    void setStatus(E_ROOM_STATUS status);
    E_ROOM_STATUS getStatus();
private:
    int id_;
    E_ROOM_STATUS status_;
    std::vector<Player> players_;   
 
};


#endif
