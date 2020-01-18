#include "server.h"

#include <iostream>
#include <unistd.h>
#include <boost/algorithm/cxx14/equal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ServerEngine.hpp"
#include <src/game/FileManager.hpp>
#include <src/game/components/Components.hpp>
#include "systems/Systems.hpp"

// do clienta
#include <src/client/MapRenderer.hpp>
#include <src/client/BasicRenderer.hpp>
#include <src/client/SelectedRenderer.hpp>

using namespace std;

bool server::i_am_the_server() {
    static const int WIDTH = 800;
    static const int HEIGHT = 600;
    ServerEngine engine(WIDTH/GridMap::TILE_SIDE, HEIGHT/GridMap::TILE_SIDE);

    std::shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Test!"));
	window->setKeyRepeatEnabled(false);
    engine.setWindow(window);

    FileManager* fileManager = FileManager::getInstance();
    fileManager->init("resources/sprites.json");

    engine.createComponentStore<Position>();
    engine.createComponentStore<Move>();
    engine.createComponentStore<Health>();
    engine.createComponentStore<Renderable>();
    engine.createComponentStore<Selectable>();

    engine.addSystem(ecs::System::Ptr(new MoveSystem(engine)));
    engine.addSystem(ecs::System::Ptr(new MapRenderer(engine)));
    engine.addSystem(ecs::System::Ptr(new BasicRenderer(engine)));
    engine.addSystem(ecs::System::Ptr(new SelectedRenderer(engine)));


    ecs::Entity entity1 = engine.createEntity();
    cout << "Entity1: " << entity1 << endl;

    engine.addComponent<Position>(entity1, Position());
    Renderable renderable(fileManager->getSprite("misiek"));
    engine.addComponent<Renderable>(entity1, std::move(renderable));
    engine.addComponent<Health>(entity1, Health(100));
    Move move(2);
    move.setDestination(0, 5);
    engine.addComponent<Move>(entity1, std::move(move));
    engine.addComponent<Selectable>(entity1, Selectable(fileManager->getSprite("selected")));

    engine.registerEntity(entity1);
    
    while(window->isOpen()){
        sf::Event event;
        while (window->pollEvent(event)) { engine.handleEvent(event); }
        window->clear(sf::Color::Black);
        engine.update(17);
        window->display();
    }

    return true;
}
