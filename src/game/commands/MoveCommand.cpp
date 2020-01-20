#include "MoveCommand.hpp"

#include <src/game/components/Components.hpp>

void MoveCommand::execute(ecs::Engine& engine) {
    auto moveStore = &engine.getComponentStore<Move>();
    auto existingEntities = engine.getEntitySet();
    for (auto &&ent : entitySet_) {
        if(existingEntities.find(ent) == existingEntities.end()) continue;
        Move* move = &moveStore->get(ent);
        move->destination.x = x_;
        move->destination.y = y_;
    }
}

sf::Packet& MoveCommand::serialize(sf::Packet& packet) {
    packet  << sf::Int32(networking::EventType::Command)
            << sf::Uint16(CommandType::MoveCommand)
            << sf::Int16(x_)
            << sf::Int16(y_)
            << sf::Uint16(entitySet_.size());

    for ( const ecs::Entity entity : entitySet_ ) {
        packet  << sf::Uint32(entity);
    }
    return packet;
}

sf::Packet& MoveCommand::deserialize(sf::Packet& packet) {
    sf::Int16 x, y;
    packet >> x >> y;
    x_ = x;
    y_ = y;

    sf::Uint16 size;
    packet >> size;

    entitySet_.clear();
    for (sf::Uint16 i=0; i<size; ++i) {
        sf::Uint32 intEnt;
        packet >> intEnt;
        entitySet_.insert((ecs::Entity)intEnt);
    }

    return packet;

}

