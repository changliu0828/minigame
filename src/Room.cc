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
        LOG_ERROR << "Add Player into Full Room.";
        return;
    }
    else
    {
        players_.push_back(player);
    } 
}
Player& Room::getAnotherPlayer(string myName) {
    std::vector<Player>::iterator it =  players_.begin(); 
    for (;it != players_.end(); it ++)
    {
        if (it->getId() != myName)
        {
            LOG_DEBUG << "Another Player: " << it->getId();
            return (*it);
        }
    } 
    return (*it);
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
   
