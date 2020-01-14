#ifndef _BASICRENDERER_
#define _BASICRENDERER_

#include <ecs/Engine.hpp>
#include <ecs/SimpleIteratingSystem.hpp>
#include <ecs/Entity.hpp>
#include <SFML/Window.hpp>
#include "../game/components/Components.hpp"

class BasicRenderer : public ecs::SimpleIteratingSystem
{
private:
    float calculatePosition(int position, int offset);
public:
    BasicRenderer(ecs::Engine &engine);
    ~BasicRenderer() {};
    void update(int64_t elapsedTime, ecs::Entity entity);

};

BasicRenderer::BasicRenderer(ecs::Engine &engine) : ecs::SimpleIteratingSystem(engine)
{
    ecs::ComponentTypeSet set;
    set.insert(Renderable::_type);
    set.insert(Position::_type);
    addRequiredComponentSet(std::move(set));
}

void BasicRenderer::update(int64_t elapsedTime, ecs::Entity entity) {
    const Position& position = _engine.getComponentStore<Position>().get(entity);
    Renderable& renderable = _engine.getComponentStore<Renderable>().get(entity);
    float x = calculatePosition(position.x, position.xoffset);
    float y = calculatePosition(position.y, position.yoffset);
    renderable.sprite->setPosition(x, y);
    ServerEngine* engine = dynamic_cast<ServerEngine*>(&_engine);
    (engine->getWindow())->draw(*renderable.sprite.get());
}

inline float BasicRenderer::calculatePosition(int position, int offset) {
    return (float)(position * GridMap::TILE_SIDE) + offset * 0.01f * GridMap::TILE_SIDE;
}

#endif