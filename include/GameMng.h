#ifndef _GAME_MNG_H_
#define _GAME_MNG_H_

#include <muduo/net/TcpConnection.h>
#include <muduo/base/Logging.h>
#include <map>
#include "Room.h"
#include "Common.h"

using namespace muduo::net;
class GameMng
{
public:
    GameMng();
    int sitDown(const TcpConnectionPtr& conn);
private:
    std::map<int, Room> emptyRooms_;
    std::map<int, Room> waitRooms_;
    std::map<int, Room> startRooms_;
};

#endif
