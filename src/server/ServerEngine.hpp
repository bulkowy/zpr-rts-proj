#ifndef _SERVERENGINE_
#define _SERVERENGINE_

#include <ecs/Engine.hpp>
#include <ecs/System.hpp>
#include "../game/GridMap.hpp"
#include <SFML/Window.hpp>
#include <memory>

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
};

#endif