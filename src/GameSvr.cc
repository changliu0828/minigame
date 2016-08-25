#include "GameSvr.h"

GameSvr::GameSvr(EventLoop* loop, const InetAddress& listenAddr)
: loop_(loop),
server_(loop, listenAddr, "GameSvr"),
codec_(boost::bind(&GameSvr::onJsonMessage, this, _1, _2, _3))
{
    server_.setConnectionCallback(
        boost::bind(&GameSvr::onConnection, this, _1));
    server_.setMessageCallback(
        boost::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
    loop->runEvery(NOTIFY_INTERVAL, boost::bind(&GameSvr::onTime, this));
}
void GameSvr::start()
{
    server_.start();
}

void GameSvr::onConnection(const TcpConnectionPtr& conn)
{
    LOG_INFO << conn->peerAddress().toIpPort() << " -> "
             << conn->localAddress().toIpPort() << " is "
             << (conn->connected() ? "UP" : "DOWN");
    if (conn->connected())
    {
        connections_.insert(conn);
    }
    else
    {
        connections_.erase(conn);
    }
 
}
void GameSvr::onJsonMessage(const TcpConnectionPtr& conn, const string& message, Timestamp time)
{  
    LOG_INFO << conn->name() << " recv " << message.size() << " bytes at " << time.toString();
}
void GameSvr::onTime()
{
    LOG_INFO << "onTime";
}
