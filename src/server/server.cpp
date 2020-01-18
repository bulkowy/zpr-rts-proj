#include "server.h"

#include <iostream>
#include <unistd.h>
#include <boost/algorithm/cxx14/equal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ServerEngine.hpp"
#include "../game/FileManager.hpp"
#include "../game/components/Components.hpp"
#include "../client/BasicRenderer.hpp"

using namespace std;

bool server::i_am_the_server() {
    ServerEngine engine(15, 15);

    std::shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(800, 600), "Test!"));
	window->setKeyRepeatEnabled(false);
    engine.setWindow(window);

    engine.addSystem(ecs::System::Ptr(new BasicRenderer(engine)));

    FileManager fileManager = FileManager();
    fileManager.init("resources/sprites.json");

    engine.createComponentStore<Position>();
    engine.createComponentStore<Move>();
    engine.createComponentStore<Health>();
    engine.createComponentStore<Renderable>();

    ecs::Entity entity1 = engine.createEntity();
    cout << "Entity1: " << entity1 << endl;

    engine.addComponent<Position>(entity1, Position());
    Renderable renderable(fileManager.getSprite("misiek"));
    engine.addComponent<Renderable>(entity1, std::move(renderable));
    engine.addComponent<Health>(entity1, Health(100));

    engine.registerEntity(entity1);
    
    while(window->isOpen()){
        sf::Event event;
        while (window->pollEvent(event)) {
                if( (event.type >= 7 && event.type <= 13) || event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus ) continue;
                // close the window on close request
                if (event.type == sf::Event::Closed) window->close();
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window->close();
            }
        window->clear(sf::Color::Black);
        engine.update(17);
        window->display();
    }

    return true;
}
