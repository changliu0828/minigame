#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <muduo/base/Types.h>
#include <muduo/net/TcpConnection.h>

using namespace muduo;
using namespace muduo::net;

class Player
{
public:
    Player(string id);
    string getId();
private:
    string id_;
};

#endif
