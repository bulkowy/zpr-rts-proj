#include "ServerEngine.hpp"

#include <src/game/systems/Systems.hpp>

ServerEngine::ServerEngine() {
    this->createComponentStore<Position>();
    this->createComponentStore<Move>();
    this->createComponentStore<Health>();
    this->createComponentStore<Renderable>();
    this->createComponentStore<Selectable>();

    this->addSystem(ecs::System::Ptr(new MoveSystem(*this)));

    ecs::Entity entity1 = this->createEntity();
    std::cout << "Entity1: " << entity1 << std::endl;

    this->addComponent<Position>(entity1, Position());
    this->addComponent<Health>(entity1, Health(100));
    Move move(2);
    move.setDestination(0, 5);
    this->addComponent<Move>(entity1, std::move(move));

    this->registerEntity(entity1);
    
    while(true) {
        update(17);
    }

}

void ServerEngine::executeCommands() {
    for (auto i = commands_.begin(); i != commands_.end(); ++i)
    {
        (*i)->execute(*this);
    }
    commands_.clear();
}

void ServerEngine::update(unsigned int frameTime) {
    executeCommands();
    Engine::update(frameTime);
}
