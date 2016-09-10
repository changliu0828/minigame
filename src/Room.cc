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
void Room::erasePlayer(string name)
{
    for (std::vector<Player>::iterator it  = players_.begin(); it != players_.end(); it++)
    {
        if (it->getID() == name)
        {
            players_.erase(it);
            return;
        }
    }   
}
void Room::eraseAllPlayer()
{
   players_.clear();
}
Player& Room::getAnotherPlayer(string myName) {
    std::vector<Player>::iterator it =  players_.begin(); 
    for (;it != players_.end(); it ++)
    {
        if (it->getID() != myName)
        {
            LOG_DEBUG << "Another Player: " << it->getID();
            return (*it);
        }
    } 
    return (*it);
}
void Room::setStatus(E_ROOM_STATUS status)
{
    status_ = status;
}
E_ROOM_STATUS Room::getStatus()
{
    return status_;
}
