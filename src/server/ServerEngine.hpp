#ifndef _SERVERENGINE_
#define _SERVERENGINE_

#include <ecs/Engine.hpp>
#include <ecs/System.hpp>
#include "../game/GridMap.hpp"
#include <SFML/Window.hpp>
#include <memory>
#include <iostream>

class ServerEngine : public ecs::Engine
{
private:
    GridMap _map;

    //do ClientEngine:
    std::shared_ptr<sf::RenderWindow> _window;
public:
    ServerEngine(int mapWidth, int mapHeight) : _map(mapWidth, mapHeight) {}
    ~ServerEngine() {}
    GridMap* getGridMap() { return &_map; }

    //do ClientEngine:
    void inline setWindow(std::shared_ptr<sf::RenderWindow> aWindow) {_window = aWindow; }
    sf::RenderWindow* getWindow() { return _window.get(); }
    void handleEvent(sf::Event&);
};

void ServerEngine::handleEvent(sf::Event &event) {
    if(event.type > 11 || event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus ) return;
    // close the window on close request
    else if (event.type == sf::Event::Closed) _window->close();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) _window->close();
    else if(event.type == sf::Event::MouseButtonPressed) {
        std::cout << "Button: " << event.mouseButton.button << ", x: " << event.mouseButton.x << ", y: " << event.mouseButton.y << std::endl;
    }
}

#endif