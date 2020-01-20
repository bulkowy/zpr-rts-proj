#ifndef _COMMAND_
#define _COMMAND_

#include <ecs/Engine.hpp>
#include <src/networking/Event.hpp>

class Command
{
public:
    Command() {}
    ~Command() {}

    virtual void execute(ecs::Engine& engine) = 0;

    friend sf::Packet& operator>>(sf::Packet& packet, Command& command);
    friend sf::Packet& operator<<(sf::Packet& packet, Command& command);
private:
    virtual sf::Packet& serialize(sf::Packet&) = 0;
    virtual sf::Packet& deserialize(sf::Packet&) = 0;
};


#endif