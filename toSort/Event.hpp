#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include "EventType.hpp"

namespace networking
{

/**
 * @brief Base class defining serialization and deserialization of communication packets
 * 
 * Every single child of this class defines one exactly type of Packet
 * and sets how serialization and deserialization of those packets takes place. 
 */
class Event
{
public:
    /**
     * @brief Construct a new Event object
     * 
     * @param type - type of event
     */
    Event(EventType);
    virtual ~Event();

    /**
     * @brief Get the type of Event
     * 
     * @return EventType 
     */
    EventType getType() const { return type_; }

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);

protected:
    EventType type_;
};

/**
 * @brief Proxy class that defines possible Connection events 
 */
class ConnectionEvent : public Event
{
public:
    /**
     * @brief Construct a new Connection Event object
     * 
     * @param event - type of event
     */
    ConnectionEvent(EventType event) : Event(event), id_(-1) {}

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);

    sf::Int32 id_; /**< ID of client that Connection Event is referring to */
};

/**
 * @brief Event defining packet informing about new Client connection
 */
class Connected : public ConnectionEvent
{
public:
    /**
     * @brief Construct a new Connected object
     */
    Connected() : 
        ConnectionEvent(EventType::Connected) {}
};

/**
 * @brief Event defining packet informing about Client disconnection
 */
class Disconnected : public ConnectionEvent 
{
public:
    /**
     * @brief Construct a new Disconnected object
     */
    Disconnected() : ConnectionEvent(EventType::Disconnected) {}
};

/**
 * @brief Event defining packet informing about new game creation
 */
class CreateGame : public Event 
{
public:
    /**
     * @brief Create a Game object
     */
    CreateGame() : Event(EventType::CreateGame) {}
};

/**
 * @brief Event defining packet informing about Client wanting to join selected game
 */
class JoinGame : public Event 
{
public:
    /**
     * @brief Default constructor of Join Game Event
     * 
     * Sets gameID_ as illegal value to be sure that it will be properly filled
     */
    JoinGame() : Event(EventType::JoinGame), gameID_(-1) {}

    /**
     * @brief Param constructor of Join Game Event
     * 
     * @param gameID - ID of selected game
     */
    JoinGame(int gameID) : Event(EventType::JoinGame), gameID_(gameID) {}

    /**
     * @brief Get Game ID
     * 
     * @return int 
     */
    int getGameID() const { return gameID_; }

    /**
     * @brief Set Game ID
     * 
     * @param gameID - ID of selected game
     */
    void setGameID(int gameID) { gameID_ = gameID; }

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);

private:
    int gameID_; /**< ID of game */
};

/**
 * @brief Event defining packet informing about existing games
 */
class GetGameList : public Event 
{
public:
    /**
     * @brief Default constructor of Get Game List Event
     */
    GetGameList() : Event(EventType::GetGameList) {}
};

/**
 * @brief Event defining packet updating list of existing games
 */
class SetGameList : public Event 
{
public:
    /**
     * @brief Default constructor of Set Game List Event
     */
    SetGameList() : Event(EventType::SetGameList) {}

    void add(int, int);

    struct Game {
        int gameID;
        int players;
    };

    sf::Uint32 size() const {
        return list_.size();
    }

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);

    std::list<SetGameList::Game> list_;
};

/**
 * @brief Event defining packet informing about new entity created or requesting creation of one
 */
class CreateEntity : public Event
{
public:
    /**
     * @brief Default constructor of Create Entity Event
     */
    CreateEntity() : Event(EventType::CreateEntity) {}

    /**
     * @brief Set members of class
     * 
     * @param id     - entity ID
     * @param posX   - pos X of entity
     * @param posY   - pos Y of entity
     */
    void set(sf::Int32 id, float posX, float posY) {
        id_ = id;
        posX_ = posX;
        posY_ = posY;
    }

    sf::Int32 id_; /**< ID of entity */
    float posX_;   /**< pos X of entity */
    float posY_;   /**< pos Y of entity */

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);
};

/**
 * @brief Event defining packet informing about update of existing entity
 */
class UpdateEntity : public Event
{
public:
    /**
     * @brief Default constructor of Create Entity Event
     */
    UpdateEntity() : Event(EventType::UpdateEntity) {}

    /**
     * @brief Set members of class
     * 
     * @param id     - entity ID
     * @param posX   - pos X of entity
     * @param posY   - pos Y of entity
     */
    void set(sf::Int32 id, float posX, float posY) {
        id_ = id;
        posX_ = posX;
        posY_ = posY;
    }

    sf::Int32 id_; /**< ID of entity */
    float posX_;   /**< pos X of entity */
    float posY_;   /**< pos Y of entity */

    /**
     * @brief Serialize Event to Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& serialize(sf::Packet&);

    /**
     * @brief Deserialize Event from Packet
     * 
     * @return sf::Packet& 
     */
    virtual sf::Packet& deserialize(sf::Packet&);
};

sf::Packet& operator>>(sf::Packet& packet, Event& event);
sf::Packet& operator<<(sf::Packet& packet, Event& event);

}

#endif