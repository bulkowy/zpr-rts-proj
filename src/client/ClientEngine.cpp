#include "ClientEngine.hpp"

#include <src/game/FileManager.hpp>
#include <src/game/systems/Systems.hpp>
#include "renderers/Renderers.hpp"
#include <src/game/commands/MoveCommand.hpp>

ClientEngine::ClientEngine() {

    std::shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(GridMap::WINDOW_WIDTH, GridMap::WINDOW_HEIGHT), "Test!"));
	window->setKeyRepeatEnabled(false);
    this->setWindow(window);

    FileManager* fileManager = FileManager::getInstance();
    fileManager->init("resources/sprites.json");

    this->createComponentStore<Position>();
    this->createComponentStore<Move>();
    this->createComponentStore<Health>();
    this->createComponentStore<Renderable>();
    this->createComponentStore<Selectable>();

    this->addSystem(ecs::System::Ptr(new MoveSystem(*this)));
    this->addSystem(ecs::System::Ptr(new MapRenderer(*this)));
    this->addSystem(ecs::System::Ptr(new DestinationRenderer(*this)));
    this->addSystem(ecs::System::Ptr(new BasicRenderer(*this)));
    this->addSystem(ecs::System::Ptr(new SelectedRenderer(*this)));

    // testing
    ecs::Entity entity1 = this->createEntity();
    createMisiek(entity1);

    ecs::Entity entity2 = createEntity();
    ecs::Entity entity3 = createEntity();
    ecs::Entity entity4 = createEntity();
    createMisiek(entity2);
    createMisiek(entity3);
    createMisiek(entity4);

}

void ClientEngine::run() {
    while(window_->isOpen()){
        sf::Event event;
        while (window_->pollEvent(event)) { if(handleEvent(event)) return; }
        window_->clear(sf::Color::Black);
        update(17);
        window_->display();
    }
}

bool ClientEngine::handleEvent(sf::Event &event) {
    if(event.type > 11 || event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus ) return false;
    // close the window on close request
    else if (event.type == sf::Event::Closed) { window_->close(); return true; }
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) { window_->close(); return true; }
    else if(event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            lmbClicked(event);
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            rmbClicked(event);
        }
    }

    return false;
}

void ClientEngine::lmbClicked(sf::Event &event) {
    auto mousePosition = window_->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    ecs::ComponentStore<Renderable>* renderables = &getComponentStore<Renderable>();
    for (auto &&renderable : renderables->getComponents())
    {
        if (renderable.second.sprite->getGlobalBounds().contains(mousePosition))
        {
            Selectable* sel = &getComponentStore<Selectable>().get(renderable.first);
            sel->selected = true;
            return;
        }  
    }
    // jeżeli żaden sprite nie został kliknięty
    ecs::ComponentStore<Selectable>* selectables = &getComponentStore<Selectable>();
    for (auto &&sel : selectables->getComponents())
    {
        sel.second.selected = false;
    }
}

void ClientEngine::rmbClicked(sf::Event &event) {
    auto mousePosition = window_->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
    std::set<ecs::Entity> selected = std::set<ecs::Entity>();
    ecs::ComponentStore<Selectable>* selectables = &getComponentStore<Selectable>();
    // dodaj wszystkie 
    for (auto &&sel : selectables->getComponents())
    {
        if (sel.second.selected)
        {
            selected.insert(sel.first);
        }
    }
    if (selected.empty())
    {
        return;
    }
    
    int x = coordsToGridCoords(mousePosition.x);
    int y = coordsToGridCoords(mousePosition.y);

    std::unique_ptr<Command> move = std::make_unique<MoveCommand>(MoveCommand(selected, x, y));
    registerCommand(std::move(move));
}

inline int ClientEngine::coordsToGridCoords(float point) { return int(point / GridMap::TILE_SIDE); }

void ClientEngine::update(unsigned int frameTime) {
    Engine::update(frameTime);
}

void ClientEngine::registerEntitySet(std::set<ecs::Entity> newEntities) {
    std::set<ecs::Entity> entities = getEntitySet();
    std::vector<ecs::Entity> difference(entities.size()); // zbiór różnicowy - najpierw starych Obiektów do usunięcia, potem nowych do dodania

    std::vector<ecs::Entity>::iterator it=std::set_difference(entities.begin(), entities.end(), newEntities.begin(), newEntities.end(), difference.begin());
    difference.resize(it-difference.begin());
    for (auto &&deadEntity : difference)
    {
        unregisterEntity(deadEntity);
    }
    
    difference.resize(newEntities.size());
    it=std::set_difference(newEntities.begin(), newEntities.end(), entities.begin(), entities.end(), difference.begin());
    difference.resize(it-difference.begin());
        for (auto &&newEntity : difference)
    {
        createMisiek(newEntity);
    }

}

void ClientEngine::createMisiek(ecs::Entity entity) {
    this->addComponent<Position>(entity, Position(0, 3*(int)entity));
    Renderable renderable(FileManager::getInstance()->getSprite("misiek"));
    this->addComponent<Renderable>(entity, std::move(renderable));
    this->addComponent<Health>(entity, Health(100));
    Move move(2);
    move.setDestination(0, 5);
    this->addComponent<Move>(entity, std::move(move));
    this->addComponent<Selectable>(entity, Selectable(FileManager::getInstance()->getSprite("selected")));

    registerEntity(entity);

}
