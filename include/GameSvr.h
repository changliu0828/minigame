#ifndef _GAMESVR_H_
#define _GAMESVR_H_

#include <muduo/net/TcpServer.h>
#include <muduo/base/AsyncLogging.h>
#include <muduo/base/Logging.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>
#include <set>
#include "Codec.h"

using namespace muduo;
using namespace muduo::net;

class GameSvr : boost::noncopyable
{
public:
    GameSvr(EventLoop* loop, const InetAddress& listenAddr);
    void start();
private:
    void onConnection(const TcpConnectionPtr& conn);
    void onJsonMessage(const TcpConnectionPtr& conn, const string& message, Timestamp time);
    typedef std::set<TcpConnectionPtr> ConnectionList;
    EventLoop* loop_;
    TcpServer server_;
    LengthHeaderCodec codec_;
    ConnectionList connections_;
};

#endif