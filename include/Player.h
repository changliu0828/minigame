#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Atom.h"
#include <muduo/base/Types.h>
#include <muduo/net/TcpConnection.h>

using namespace muduo;
using namespace muduo::net;

class Player : public Atom
{
public:
    Player(string id);
private:
    string id_;
    string name_;
};

#endif
