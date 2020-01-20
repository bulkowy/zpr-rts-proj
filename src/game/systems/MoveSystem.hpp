#ifndef _MOVESYSTEM_
#define _MOVESYSTEM_

#include <boost/math/special_functions/sign.hpp>
#include <ecs/SimpleIteratingSystem.hpp>
#include <ecs/Entity.hpp>

#include <src/game/components/Position.hpp>
#include <src/game/components/Move.hpp>
// struct Move;
// struct Position;

class MoveSystem : public ecs::SimpleIteratingSystem
{
private:
    ecs::ComponentStore<Position>* _positionComponents;
    ecs::ComponentStore<Move>* _moveComponents;
public:
    MoveSystem(ecs::Engine engine);
    ~MoveSystem() {}
    void update(unsigned int frameLength, ecs::Entity entity);
};


MoveSystem::MoveSystem(ecs::Engine engine) : ecs::SimpleIteratingSystem(engine) {
ecs::ComponentTypeSet set;
set.insert(Position::_type);
set.insert(Move::_type);
addRequiredComponentSet(std::move(set));

_positionComponents = &_engine.getComponentStore<Position>();
_moveComponents = &_engine.getComponentStore<Move>();
}

void MoveSystem::update(unsigned int frameLength, ecs::Entity entity) {
    auto position = &(_positionComponents->get(entity));
    auto move = &_moveComponents->get(entity);

    std::pair<int8_t, int8_t> direction;
    direction.first = boost::math::sign<int>(move->destination.x - position->x);
    direction.second = boost::math::sign<int>(move->destination.y - position->y);

    position->xoffset += direction.first * move->speed * frameLength / 10.0; // dzielone przez 1000 żeby uzyskać [ms], * 100% -> w efekcie dzielone przez 10
    position->yoffset += direction.second * move->speed * frameLength / 10.0;

    if (std::abs(position->xoffset) > 100)
    {
        position->x += 1*direction.first;
        position->xoffset = 0;
    }
    if (std::abs(position->yoffset) > 100)
    {
        position->y += 1*direction.second;
        position->yoffset = 0;
    }
    
}
#endif