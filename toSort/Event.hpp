#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>
#include <list>
#include "EventType.hpp"

namespace networking
{

class Event
{
public:
    Event(EventType);
    virtual ~Event();

    EventType getType() const { return type_; }

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);

protected:
    EventType type_;
};

class ConnectionEvent : public Event
{
public:
    ConnectionEvent(EventType event) : Event(event), id_(-1) {}

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);


    sf::Int32 id_;
};


class Connected : public ConnectionEvent
{
public:
    Connected() : 
        ConnectionEvent(EventType::Connected) {}
};

class Disconnected : public ConnectionEvent 
{
public:
    Disconnected() : ConnectionEvent(EventType::Disconnected) {}
};

class CreateGame : public Event 
{
public:
    CreateGame() : Event(EventType::CreateGame) {}
};

class JoinGame : public Event 
{
public:
    JoinGame() : Event(EventType::JoinGame), gameID_(-1) {}
    JoinGame(int gameID) : Event(EventType::JoinGame), gameID_(gameID) {}

    int getGameID() const { return gameID_; }
    void setGameID(int gameID) { gameID_ = gameID; }

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);

private:
    int gameID_;
};

class GetGameList : public Event 
{
public:
    GetGameList() : Event(EventType::GetGameList) {}
};

class SetGameList : public Event 
{
public:
    SetGameList() : Event(EventType::SetGameList) {}

    void add(int, int);

    struct Game {
        int gameID;
        int players;
    };

    sf::Uint32 size() const {
        return list_.size();
    }

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);

    std::list<SetGameList::Game> list_;
};

class CreateEntity : public Event
{
public:
    CreateEntity() : Event(EventType::CreateEntity) {}

    void set(sf::Int32 id, float posX, float posY) {
        id_ = id;
        posX_ = posX;
        posY_ = posY;
    }

    sf::Int32 id_;
    float posX_;
    float posY_;

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);
};

class UpdateEntity : public Event
{
public:
    UpdateEntity() : Event(EventType::UpdateEntity) {}

    void set(sf::Int32 id, float posX, float posY) {
        id_ = id;
        posX_ = posX;
        posY_ = posY;
    }

    sf::Int32 id_;
    float posX_;
    float posY_;

    virtual sf::Packet& serialize(sf::Packet&);
    virtual sf::Packet& deserialize(sf::Packet&);
};

sf::Packet& operator>>(sf::Packet& packet, Event& event);
sf::Packet& operator<<(sf::Packet& packet, Event& event);

}

#endif