#include "Command.hpp"

sf::Packet& operator<<(sf::Packet& packet, Command& command) { return command.serialize(packet); }
sf::Packet& operator>>(sf::Packet& packet, Command& command) { return command.deserialize(packet); }
