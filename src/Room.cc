#include "Room.h"

Room::Room(int id)
:id_(id)
{
    init();
}
void Room::init()
{
    status_ = ROOM_EMPTY; 
    players_.clear();
}
void Room::addPlayer(Player player) 
{
    if (players_.size() == PLAYER_NUM_PER_ROOM)
    {
        return;
    }
    else
    {
        players_.push_back(player);
    } 
}
void Room::Start()
{
    status_ = ROOM_START;
    //TODO:send client with born pos 
}
void Room::Wait()
{
    status_ = ROOM_WAIT;
}
void Room::Empty()
{
    status_ = ROOM_EMPTY;
}
