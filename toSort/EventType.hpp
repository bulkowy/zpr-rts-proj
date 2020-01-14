#ifndef EVENTTYPE_HPP_
#define EVENTTYPE_HPP_

namespace networking 
{

enum class EventType
{
    Connected,
    Disconnected,
    CreateGame,
    JoinGame,
    GetGameList,
    SetGameList,
    UpdateEntity,
    CreateEntity,
};

}

#endif