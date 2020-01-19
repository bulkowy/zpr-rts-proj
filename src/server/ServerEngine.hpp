#ifndef _SERVERENGINE_
#define _SERVERENGINE_

#include <memory>
#include <iostream>
#include <vector>

#include <SFML/Window.hpp>
#include <ecs/Engine.hpp>
#include <ecs/System.hpp>
#include <src/game/GridMap.hpp>
#include <src/game/commands/MoveCommand.hpp>

class ServerEngine : public ecs::Engine
{
private:
    GridMap _map;
    std::vector<std::unique_ptr<Command>> _commands;

    void executeCommands();

    //do ClientEngine:
    std::shared_ptr<sf::RenderWindow> _window;
public:
    ServerEngine(int mapWidth, int mapHeight) : _map(mapWidth, mapHeight) {}
    ~ServerEngine() {}
    GridMap* getGridMap() { return &_map; }
    void registerCommand(std::unique_ptr<Command> command) {_commands.push_back(std::move(command)); }

    inline void update(int64_t frameTime) override {
        executeCommands();
        Engine::update(frameTime);
    }

    //do ClientEngine:
    void inline setWindow(std::shared_ptr<sf::RenderWindow> aWindow) {_window = aWindow; }
    sf::RenderWindow* getWindow() { return _window.get(); }
    void handleEvent(sf::Event&);
    void lmbClicked(sf::Event&);
    void rmbClicked(sf::Event&);
    int coordsToGridCoords(float);
};

void ServerEngine::executeCommands() {
    for (auto i = _commands.begin(); i != _commands.end(); ++i)
    {
        (*i)->execute(*this);
    }
    _commands.clear();
}

// wszystko poniżej do ClientEngine
void ServerEngine::handleEvent(sf::Event &event) {
    if(event.type > 11 || event.type == sf::Event::LostFocus || event.type == sf::Event::GainedFocus ) return;
    // close the window on close request
    else if (event.type == sf::Event::Closed) _window->close();
    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) _window->close();
    else if(event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            lmbClicked(event);
        }
        else if (event.mouseButton.button == sf::Mouse::Right) {
            rmbClicked(event);
        }
    }
}

void ServerEngine::lmbClicked(sf::Event &event) {
    auto mousePosition = _window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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

void ServerEngine::rmbClicked(sf::Event &event) {
    auto mousePosition = _window->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
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

inline int ServerEngine::coordsToGridCoords(float point) { return int(point / GridMap::TILE_SIDE); }


#endif