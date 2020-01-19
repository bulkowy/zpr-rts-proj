#ifndef _DESTINATIONRENDERER_
#define _DESTINATIONRENDERER_

#include <ecs/Engine.hpp>
#include <ecs/SimpleIteratingSystem.hpp>
#include <ecs/Entity.hpp>
#include <SFML/Window.hpp>
#include "../game/components/Components.hpp"

class DestinationRenderer : public ecs::SimpleIteratingSystem
{
private:
    float calculatePosition(int position);
public:
    DestinationRenderer(ecs::Engine &engine);
    ~DestinationRenderer() {};
    void update(int64_t elapsedTime, ecs::Entity entity);

};

DestinationRenderer::DestinationRenderer(ecs::Engine &engine) : ecs::SimpleIteratingSystem(engine)
{
    ecs::ComponentTypeSet set;
    set.insert(Selectable::_type);
    set.insert(Position::_type);
    set.insert(Move::_type);
    addRequiredComponentSet(std::move(set));
}

void DestinationRenderer::update(int64_t elapsedTime, ecs::Entity entity) {
    Selectable& selectable = _engine.getComponentStore<Selectable>().get(entity);
    if(!selectable.selected) return;
    const Position& position = _engine.getComponentStore<Position>().get(entity);
    const Move& move = _engine.getComponentStore<Move>().get(entity);
    if(move.destination.x == position.x && move.destination.y == position.y) return;

    float x = calculatePosition(move.destination.x);
    float y = calculatePosition(move.destination.y);
    selectable.destinationSprite->setPosition(x, y);
    ServerEngine* engine = dynamic_cast<ServerEngine*>(&_engine);
    (engine->getWindow())->draw(*selectable.destinationSprite.get());
}

inline float DestinationRenderer::calculatePosition(int position) {
    return (float)(position * GridMap::TILE_SIDE);
}

#endif