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
void GameMng::sitDown(const TcpConnectionPtr& conn, const ptree &jsontree)
{
    string name;
    try
    {
        name = jsontree.get<string>("name");
    }
    catch (const ptree_error &e)
    {
        name = "Pony Ma";
        LOG_ERROR << "GameMng::sitDown > Failed to read player's name";
    }  
    
    LOG_DEBUG << "sit Down: Player: " << name << " at Conn: " << conn->name(); 

    if (mapConnNameRoomID_.find(conn->name()) != mapConnNameRoomID_.end())
    {
        LOG_ERROR << "GameMng::sitDown > Player: " << name << " already in Room: " << mapConnNameRoomID_[conn->name()]; 
        return;
    } 

    if (!waitRooms_.empty())
    {
        //get a Room from wait-rooms and insert into start-rooms
        std::pair<int, Room> tmppair = *(waitRooms_.begin());
        Player player(conn->name());
        player.setName(name);
        tmppair.second.addPlayer(player);
        tmppair.second.setStatus(ROOM_START);
        
        waitRooms_.erase(waitRooms_.begin());
        startRooms_.insert(tmppair);
        
        mapConnNameRoomID_.insert(std::pair<muduo::string, int>(conn->name(), tmppair.first));

        //send to slave
        rspSitDown(conn,ROOM_START, false);

        // send to master 
        std::map<int, Room>::iterator it = startRooms_.find(tmppair.first);
        string otherName = it->second.getAnotherPlayer(conn->name()).getID();
        TcpConnectionPtr otherConn = connections_[otherName];

        rspSitDown(otherConn,ROOM_START, true);
  
        LOG_INFO << "Player: " << conn->name() << " Sit Down in Waiting Room: " << tmppair.first; 
    }
    else if (!emptyRooms_.empty())
    {
        //get a Room from emptyrooms and insert into startrooms
        std::pair<int, Room> tmppair = *(emptyRooms_.begin());
        Player player(conn->name());
        player.setName(name);
        tmppair.second.addPlayer(player);
        tmppair.second.setStatus(ROOM_WAIT);
       
        
        emptyRooms_.erase(emptyRooms_.begin());
        waitRooms_.insert(tmppair);

        rspSitDown(conn,ROOM_WAIT);

        mapConnNameRoomID_.insert(std::pair<muduo::string, int>(conn->name(), tmppair.first));
        LOG_INFO << "Player: " << conn->name() << " Sit Down in Empty Room: " << tmppair.first; 
    }
    else
    {
        LOG_INFO << "No Enough Rooms for " << conn->name();  
        rspEvent(conn, EVENT_NO_EMPTY_ROOM);
    }
}
void GameMng::connUp(const TcpConnectionPtr& conn) 
{
    connections_.insert(std::pair<string, TcpConnectionPtr>(conn->name(), conn));
}
void GameMng::connDown(const TcpConnectionPtr& conn) 
{
    std::map<muduo::string, int>::iterator itRoomID;
    //Connection is in a room.
    if ((itRoomID = mapConnNameRoomID_.find(conn->name())) != mapConnNameRoomID_.end())
    {
        std::map<int, Room>::iterator itRoom ;
        
        if ((itRoom = startRooms_.find(itRoomID->second)) != startRooms_.end())
        {
            LOG_INFO << "connDown > Connection is in a start-room";
            //notify the other player
            string partnerID = itRoom->second.getAnotherPlayer(conn->name()).getID();
            TcpConnectionPtr partnerConn = connections_[partnerID];
            rspEvent(partnerConn, EVENT_PARTNER_QUIT);
            
            itRoom->second.eraseAllPlayer();
            itRoom->second.setStatus(ROOM_EMPTY);
            
            emptyRooms_.insert(*itRoom);
            startRooms_.erase(itRoom);
        }
        else if ((itRoom = waitRooms_.find(itRoomID->second)) != waitRooms_.end())
        {
            LOG_INFO << "connDown > Connection is in a wait-room";
            itRoom->second.eraseAllPlayer();
            itRoom->second.setStatus(ROOM_EMPTY);

            emptyRooms_.insert(*itRoom);
            waitRooms_.erase(itRoom);
        }
        else if ((itRoom = emptyRooms_.find(itRoomID->second)) != emptyRooms_.end())
        {
            LOG_INFO << "connDown > Connection is in a empty-room";
        }
        else
        {
            LOG_ERROR << "connDown > Connection is not in a room";
        }
        mapConnNameRoomID_.erase(itRoomID);
    }
    connections_.erase(conn->name());
}
void GameMng::rspSitDown(const TcpConnectionPtr& conn, E_ROOM_STATUS status, bool isMaster)
{
    boost::property_tree::ptree jsontree;
    jsontree.put("msg_type", MSG_SITDOWN_RSP);
    jsontree.put("room_status", status);
    jsontree.put("isMaster", isMaster);
    codecPtr_->send(conn, jsontree);
}
void GameMng::rspFrame(const TcpConnectionPtr& conn, ptree& jsontree)
{
    if (!conn) 
    {
        LOG_ERROR<<"TcpConnectionPtr is NULL";
        return;
    }
    if (connections_.find(conn->name()) == connections_.end())
    {
        LOG_ERROR<<"Connection does not exist: "<<conn->name();
        return;
    }
    if (mapConnNameRoomID_.find(conn->name()) == mapConnNameRoomID_.end())
    {
        LOG_ERROR<<"Connection is not in Room: "<<conn->name();
        return;
    }
    int roomID = mapConnNameRoomID_[conn->name()];

    std::map<int, Room>::iterator it = startRooms_.find(roomID);
   
    if (it == startRooms_.end())
    {
        LOG_ERROR<<"Room does not start";
        return;
    }

    string partnerID = it->second.getAnotherPlayer(conn->name()).getID();
    TcpConnectionPtr partnerConn = connections_[partnerID];
    //modify the msg_type
    //jsontree.put("msg_type", MSG_FRAME_RSP);

    codecPtr_->send(partnerConn, jsontree);
}
void GameMng::rspEvent(const TcpConnectionPtr& conn, E_EVENT_TYPE event)
{
    if (!conn) 
    {
        LOG_ERROR<<"TcpConnectionPtr is NULL";
        return;
    }
    boost::property_tree::ptree jsontree;
    jsontree.put("msg_type", MSG_EVENT_RSP);
    jsontree.put("event_type", event);
    codecPtr_->send(conn, jsontree);
}
