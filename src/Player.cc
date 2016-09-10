#include "Player.h"

Player::Player(string id)
:id_(id)
{
    name_ = "default_name";   
}
string Player::getID()
{
    return id_; 
}
void Player::setName(string name)
{
    name_ = name;
}
string Player::getName()
{
    return name_;
}
