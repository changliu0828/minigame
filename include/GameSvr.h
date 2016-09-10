#ifndef _GAMESVR_H_
#define _GAMESVR_H_

#include <muduo/net/TcpServer.h>
#include <muduo/base/AsyncLogging.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Codec.h"
#include "Common.h"
#include "GameMng.h"

using namespace muduo;
using namespace muduo::net;
using namespace boost::property_tree;

class GameSvr : boost::noncopyable
{
public:
    GameSvr(EventLoop* loop, const InetAddress& listenAddr);
    void start();
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onJsonMessage(const TcpConnectionPtr& conn, ptree& jsontree, Timestamp time);
    void onTime();
    void doByMsgType(int msgType, const TcpConnectionPtr& conn, ptree& jsontree);
    void doSitDown(const TcpConnectionPtr& conn, const ptree& jsontree);
    void doFrame(const TcpConnectionPtr& conn, ptree& jsontree);
    EventLoop* loop_;
    TcpServer server_;
    LengthHeaderCodec codec_;
    GameMng gameMng_;
};

#endif
