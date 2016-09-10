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
 //   loop->runEvery(1, boost::bind(&GameSvr::onTime, this));
    LengthHeaderCodec* pCodec = &codec_;
    boost::shared_ptr<LengthHeaderCodec> codecPtr(pCodec);
    this->gameMng_.setCodec(codecPtr);
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
        gameMng_.connUp(conn);
    }
    else
    {
        gameMng_.connDown(conn);         
    }
 
}
void GameSvr::onJsonMessage(const TcpConnectionPtr& conn, ptree& jsontree, Timestamp time)
{  
    int msgType = jsontree.get<int>("msg_type");
    LOG_INFO << conn->name() << " msg_type: " << msgType  << " at " << time.toString();

    doByMsgType(msgType, conn, jsontree);
}
void GameSvr::onTime()
{
    LOG_INFO << "onTime";
}
void GameSvr::doByMsgType(int msgType, const TcpConnectionPtr& conn, ptree &jsontree)
{
    switch (msgType) 
    {
        case MSG_SITDOWN_REQ:
        {
            doSitDown(conn, jsontree);
            break;
        }
        case MSG_FRAME_REQ:
        {
            doFrame(conn, jsontree);
            break;
        }
    }   
}
void GameSvr::doSitDown(const TcpConnectionPtr& conn, const ptree &jsontree)
{
    gameMng_.sitDown(conn, jsontree);
}
void GameSvr::doFrame(const TcpConnectionPtr& conn, ptree &jsontree)
{
    gameMng_.reqFrame(conn, jsontree);
}

