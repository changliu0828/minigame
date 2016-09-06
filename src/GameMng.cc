#include "GameMng.h"

GameMng::GameMng()
{
    for (int i = 0; i < MAX_ROOM_NUM; ++ i) {
        Room room(i);
        std::pair<int, Room> tmppair(i, room);
        emptyRooms_.insert(tmppair);  
    } 
}
void GameMng::setCodec(LengthHeaderCodecPtr codecPtr)
{
    this->codecPtr_ = codecPtr;
}
void GameMng::sitDown(const TcpConnectionPtr& conn)
{
    if (mapConnNameRoomID_.find(conn->name()) != mapConnNameRoomID_.end())
    {
        LOG_INFO << "Player: " << conn->name() << " Already In Room: " << mapConnNameRoomID_[conn->name()]; 
        return;
    } 

    if (!waitRooms_.empty())
    {
        //get a Room from waitrooms and insert into startrooms
        std::pair<int, Room> tmppair = *(waitRooms_.begin());
        Player player(conn->name());
        tmppair.second.addPlayer(player);
        tmppair.second.Start();
        
        waitRooms_.erase(waitRooms_.begin());
        startRooms_.insert(tmppair);
        
        mapConnNameRoomID_.insert(std::pair<muduo::string, int>(conn->name(), tmppair.first));
        LOG_INFO << "Player: " << conn->name() << " Sit Down in Waiting Room: " << tmppair.first; 
    }
    else if (!emptyRooms_.empty())
    {
        //get a Room from emptyrooms and insert into startrooms
        std::pair<int, Room> tmppair = *(emptyRooms_.begin());
        Player player(conn->name());
        tmppair.second.addPlayer(player);
        tmppair.second.Wait();
        
        emptyRooms_.erase(emptyRooms_.begin());
        waitRooms_.insert(tmppair);

        mapConnNameRoomID_.insert(std::pair<muduo::string, int>(conn->name(), tmppair.first));
        LOG_INFO << "Player: " << conn->name() << " Sit Down in Empty Room: " << tmppair.first; 
    }
    else
    {
        LOG_INFO << "No Enough Rooms for " << conn->name();  
        //TODO:send no enough rooms to client
    }
}
void GameMng::connUp(const TcpConnectionPtr& conn) 
{
    connections_.insert(std::pair<string, TcpConnectionPtr>(conn->name(), conn));
}
void GameMng::connDown(const TcpConnectionPtr& conn) 
{
    connections_.erase(conn->name());
}
void GameMng::reqFrame(const TcpConnectionPtr& conn, const ptree& jsontree)
{
    if (connections_.find(conn->name()) == connections_.end())
    {
        LOG_ERROR<<"Connection not exist: "<<conn->name();
        return;
    }
    if (mapConnNameRoomID_.find(conn->name()) == mapConnNameRoomID_.end())
    {
        LOG_ERROR<<"Connection not in Room: "<<conn->name();
        return;
    }
    int roomID = mapConnNameRoomID_[conn->name()];

    std::map<int, Room>::iterator it = startRooms_.find(roomID);
   
    if (it == startRooms_.end())
    {
        LOG_ERROR<<"Room does not start.";
        return;
    }

    string otherName = it->second.getAnotherPlayer(conn->name()).getId();
    TcpConnectionPtr otherConn = connections_[otherName];
    LOG_INFO<<"other Conn"<<otherConn->name();
    codecPtr_->send(otherConn, jsontree);
}
