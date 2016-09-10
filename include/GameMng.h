#ifndef _GAME_MNG_H_
#define _GAME_MNG_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <muduo/net/TcpConnection.h>
#include <map>
#include "Room.h"
#include "Common.h"
#include "Codec.h"

using namespace muduo::net;
using namespace boost::property_tree;
typedef std::map<string, TcpConnectionPtr> ConnectionMap;

class GameMng
{
public:
    GameMng();
    void setCodec(LengthHeaderCodecPtr codecPtr);
    void sitDown(const TcpConnectionPtr& conn, const ptree& jsontree);
    void connUp(const TcpConnectionPtr& conn);
    void connDown(const TcpConnectionPtr& conn);
    void reqSitDown(const TcpConnectionPtr& conn, E_ROOM_STATUS status, bool isMaster = false);
    void reqFrame(const TcpConnectionPtr& conn, ptree& jsontree);
    void reqEvent(const TcpConnectionPtr& conn, E_EVENT_TYPE event);

private:
    LengthHeaderCodecPtr codecPtr_;
    ConnectionMap connections_;
    std::map<int, Room> emptyRooms_;
    std::map<int, Room> waitRooms_;
    std::map<int, Room> startRooms_;
    std::map<muduo::string, int> mapConnNameRoomID_;
};

#endif
