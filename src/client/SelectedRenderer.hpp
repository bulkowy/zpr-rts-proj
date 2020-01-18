#ifndef _SELECTEDRENDERER_
#define _SELECTEDRENDERER_

#include <ecs/Engine.hpp>
#include <ecs/SimpleIteratingSystem.hpp>
#include <ecs/Entity.hpp>
#include <SFML/Window.hpp>
#include "../game/components/Components.hpp"

class SelectedRenderer : public ecs::SimpleIteratingSystem
{
private:
    float calculatePosition(int position, int offset);
public:
    SelectedRenderer(ecs::Engine &engine);
    ~SelectedRenderer() {};
    void update(int64_t elapsedTime, ecs::Entity entity);

};

SelectedRenderer::SelectedRenderer(ecs::Engine &engine) : ecs::SimpleIteratingSystem(engine)
{
    ecs::ComponentTypeSet set;
    set.insert(Selectable::_type);
    set.insert(Position::_type);
    addRequiredComponentSet(std::move(set));
}

void SelectedRenderer::update(int64_t elapsedTime, ecs::Entity entity) {
    Selectable& selectable = _engine.getComponentStore<Selectable>().get(entity);
    if(!selectable.selected) return;
    const Position& position = _engine.getComponentStore<Position>().get(entity);
    float x = calculatePosition(position.x, position.xoffset);
    float y = calculatePosition(position.y, position.yoffset);
    selectable.selectedSprite->setPosition(x, y);
    ServerEngine* engine = dynamic_cast<ServerEngine*>(&_engine);
    (engine->getWindow())->draw(*selectable.selectedSprite.get());
}

inline float SelectedRenderer::calculatePosition(int position, int offset) {
    return (float)(position * GridMap::TILE_SIDE) + offset * 0.01f * GridMap::TILE_SIDE;
}

#endif