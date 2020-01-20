#ifndef _MOVECOMMAND_
#define _MOVECOMMAND_

#include <set>

#include "Command.hpp"
#include <ecs/Entity.hpp>
#include <ecs/Engine.hpp>

class MoveCommand : public Command {
private:
    std::set<ecs::Entity> entitySet_;
    int x_;
    int y_;
public:
    MoveCommand() {}
    MoveCommand(std::set<ecs::Entity> &entities, int &x, int &y) : entitySet_(entities), x_(x), y_(y) {}
    ~MoveCommand() {}
    void execute(ecs::Engine& engine);

    sf::Packet& serialize(sf::Packet&);
    sf::Packet& deserialize(sf::Packet&);

};

#endif