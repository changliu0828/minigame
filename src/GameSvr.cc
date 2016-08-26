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
    loop->runEvery(1, boost::bind(&GameSvr::onTime, this));
    
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
void GameSvr::onJsonMessage(const TcpConnectionPtr& conn, const ptree& jsontree, Timestamp time)
{  
    int msgType = jsontree.get<int>("MSG_TYPE");
    LOG_INFO << conn->name() << " MSG_TYPE: " << msgType  << "at " << time.toString();

    doByMsgType(msgType, conn);
}
void GameSvr::onTime()
{
    LOG_INFO << "onTime";
}
void GameSvr::doByMsgType(int msgType, const TcpConnectionPtr& conn)
{
    switch (msgType) 
    {
        case MSG_MATCH_REQ:
        {
            doReqMatch(conn);
            break;
        }
        case MSG_FRAME_REQ:
        {
            break;
        }
    }   
}
void GameSvr::doReqMatch(const TcpConnectionPtr& conn)
{
    gameMng_.sitDown(conn);
}
