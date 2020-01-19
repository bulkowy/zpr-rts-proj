#ifndef _MOVECOMMAND_
#define _MOVECOMMAND_

#include <set>

#include "Command.hpp"
#include <ecs/Entity.hpp>
#include <ecs/Engine.hpp>
#include <src/game/components/Components.hpp>

class MoveCommand : public Command {
private:
    std::set<ecs::Entity> _entitySet;
    int _x;
    int _y;
public:
    MoveCommand(std::set<ecs::Entity> &entities, int &x, int &y) : _entitySet(entities), _x(x), _y(y) {}
    ~MoveCommand() {}
    void execute(ecs::Engine& engine) {
        auto moveStore = &engine.getComponentStore<Move>();
        for (auto &&ent : _entitySet) {
            Move* move = &moveStore->get(ent);
            move->destination.x = _x;
            move->destination.y = _y;
        }
    }

    std::unique_ptr<Command> clone() { return std::make_unique<MoveCommand>(*this); }
};

#endif