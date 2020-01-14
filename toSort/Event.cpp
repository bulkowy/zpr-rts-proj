#include "Event.hpp"

#include <iostream>
namespace networking {

Event::Event(EventType type) : type_(type) {} 
Event::~Event() {}

sf::Packet& operator>>(sf::Packet& packet, Event& event) { 
    return event.deserialize(packet);
}
sf::Packet& operator<<(sf::Packet& packet, Event& event) {
    return event.serialize(packet);
}


sf::Packet& Event::serialize(sf::Packet& packet) {
    packet << sf::Int32(getType());
    return packet;
}

sf::Packet& Event::deserialize(sf::Packet& packet) {
    return packet;
}

sf::Packet& ConnectionEvent::serialize(sf::Packet& packet) {
    packet  << sf::Int32(getType())   
            << id_;
    return packet;
}

sf::Packet& ConnectionEvent::deserialize(sf::Packet& packet) {
    packet >> id_;
    return packet;
}

sf::Packet& JoinGame::serialize(sf::Packet& packet) {
    packet  << sf::Int32(getType())   
            << sf::Int32(getGameID());
    return packet;
}

sf::Packet& JoinGame::deserialize(sf::Packet& packet) {
    sf::Int32 gameID;
    packet >> gameID;
    setGameID(gameID);
    return packet;
}

sf::Packet& SetGameList::serialize(sf::Packet& packet) {
    packet  << sf::Int32(getType()) 
            << sf::Uint32(size());

    for ( const SetGameList::Game& game : list_ ) {
        packet  << sf::Int32(game.gameID) 
                << sf::Int32(game.players);
    }

    return packet;
}

sf::Packet& SetGameList::deserialize(sf::Packet& packet) {
    sf::Uint32 size;
    packet >> size;
    list_.clear();
    for ( sf::Uint32 i = 0; i < size; i++ ) {
        sf::Int32 gameID;
        sf::Int32 players;

        packet  >> gameID 
                >> players;

        SetGameList::Game game = { 
            .gameID = gameID, 
            .players = players 
        };

        list_.emplace_back(std::move(game));
    }
    return packet;
}

sf::Packet& CreateEntity::serialize(sf::Packet& packet) {
    packet << sf::Int32(type_)
           << id_
           << posX_
           << posY_;
    
    return packet;
}

sf::Packet& CreateEntity::deserialize(sf::Packet& packet) {
    packet >> id_ >> posX_ >> posY_;
    return packet;
}

sf::Packet& UpdateEntity::serialize(sf::Packet& packet) {
    packet << sf::Int32(type_)
           << id_
           << posX_
           << posY_;
    
    return packet;
}

sf::Packet& UpdateEntity::deserialize(sf::Packet& packet) {
    packet >> id_ >> posX_ >> posY_;
    return packet;
}

void SetGameList::add(int gameID, int players) {
    SetGameList::Game tmp = { 
        .gameID = gameID, 
        .players = players 
    };
    list_.emplace_back(std::move(tmp));
}

}