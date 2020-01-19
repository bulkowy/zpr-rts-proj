#ifndef __EVENTTYPE_HPP_
#define __EVENTTYPE_HPP_

namespace networking 
{

/**
 * @brief Enum defining possible packets in communication
 */
enum class EventType
{
    Connected,
    Disconnected,
    CreateGame,
    JoinGame,
    GetGameList,
    SetGameList,
    MoveCommand
};

}

#endif