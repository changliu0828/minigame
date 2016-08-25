#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Atom.h"
#include <muduo/base/Types.h>
using namespace muduo;

class Player : public Atom
{
public:
    Player();
private:
    string id_; // = TcpConnetciont.name()
    string name_;
};

#endif
