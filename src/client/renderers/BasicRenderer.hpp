#ifndef _BASICRENDERER_
#define _BASICRENDERER_

#include <ecs/Engine.hpp>
#include <ecs/SimpleIteratingSystem.hpp>
#include <ecs/Entity.hpp>
#include <SFML/Window.hpp>
#include <src/game/components/Components.hpp>
#include "../ClientEngine.hpp"

class BasicRenderer : public ecs::SimpleIteratingSystem
{
private:
    float calculatePosition(int position, int offset);
public:
    BasicRenderer(ecs::Engine &engine);
    ~BasicRenderer() {};
    void update(unsigned int elapsedTime, ecs::Entity entity);

};

BasicRenderer::BasicRenderer(ecs::Engine &engine) : ecs::SimpleIteratingSystem(engine)
{
    ecs::ComponentTypeSet set;
    set.insert(Renderable::_type);
    set.insert(Position::_type);
    addRequiredComponentSet(std::move(set));
}

void BasicRenderer::update(unsigned int elapsedTime, ecs::Entity entity) {
    const Position& position = _engine.getComponentStore<Position>().get(entity);
    Renderable& renderable = _engine.getComponentStore<Renderable>().get(entity);
    float x = calculatePosition(position.x, position.xoffset);
    float y = calculatePosition(position.y, position.yoffset);
    renderable.sprite->setPosition(x, y);
    ClientEngine* engine = dynamic_cast<ClientEngine*>(&_engine);
    (engine->getWindow())->draw(*renderable.sprite.get());
}

inline float BasicRenderer::calculatePosition(int position, int offset) {
    return (float)(position * GridMap::TILE_SIDE) + offset * 0.01f * GridMap::TILE_SIDE;
}

#endif