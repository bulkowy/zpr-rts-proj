#include "Move.hpp"

#include <SFML/Network.hpp>
#include <src/networking/EventType.hpp>

const ecs::ComponentType Move::_type = 2;


sf::Packet& operator>>(sf::Packet& packet, Move& move) {
    return move.deserialize(packet);
}
sf::Packet& operator<<(sf::Packet& packet, Move& move) {
    return move.serialize(packet);
}

sf::Packet& Move::serialize(sf::Packet& packet) {
    packet  << sf::Int32(destination.x)
            << sf::Int32(destination.y);
    return packet;
}
sf::Packet& Move::deserialize(sf::Packet& packet) {
    packet >> destination.x >> destination.y;
    return packet;
}
