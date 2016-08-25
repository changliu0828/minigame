#include "Room.h"

Room::Room()
:id_(0)
{
    init();
}
void Room::init()
{
    status_ = ROOM_EMPTY; 
    players_.clear();
}
