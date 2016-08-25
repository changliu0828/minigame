#include "Room.h"

Room::Room()
{
    init();
}
void Room::init()
{
    status_ = ROOM_EMPTY; 
    players_.clear();
}
