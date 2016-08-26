#include "GameMng.h"

GameMng::GameMng()
{
    for (int i = 0; i < MAX_ROOM_NUM; ++ i) {
        Room room(i);
        std::pair<int, Room> tmppair(i, room);
        emptyRooms_.insert(tmppair);  
    } 
}
int GameMng::sitDown(const TcpConnectionPtr& conn)
{
    if (!waitRooms_.empty())
    {
        //get a Room from waitrooms and insert into startrooms
        std::pair<int, Room> tmppair = *(waitRooms_.begin());
        waitRooms_.erase(waitRooms_.begin());
        startRooms_.insert(tmppair);
        
        Player player(conn->name());
        tmppair.second.addPlayer(player);
        tmppair.second.Start();
    }
    else if (!emptyRooms_.empty())
    {
        //get a Room from emptyrooms and insert into startrooms
        std::pair<int, Room> tmppair = *(emptyRooms_.begin());
        emptyRooms_.erase(emptyRooms_.begin());
        startRooms_.insert(tmppair);

        Player player(conn->name());
        tmppair.second.addPlayer(player);
        tmppair.second.Wait();
    }
    else
    {
        LOG_INFO << "No Enough Rooms for " << conn->name();  
    }
    return 0;
}
