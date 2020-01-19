#ifndef _CLIENTENGINE_
#define _CLIENTENGINE_

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ecs/Engine.hpp>
#include <src/game/GridMap.hpp>
#include <src/game/commands/Command.hpp>



class ClientEngine : public ecs::Engine {
private:
    GridMap map_;
    std::shared_ptr<sf::RenderWindow> window_;
    std::vector<std::unique_ptr<Command>> commands_;
public:
    ClientEngine();
    GridMap* getGridMap() { return &map_; }

    void inline setWindow(std::shared_ptr<sf::RenderWindow> aWindow) {window_ = aWindow; }
    sf::RenderWindow* getWindow() { return window_.get(); }
    void handleEvent(sf::Event&);
    void lmbClicked(sf::Event&);
    void rmbClicked(sf::Event&);
    int coordsToGridCoords(float);

    void registerCommand(std::unique_ptr<Command> command) {commands_.push_back(std::move(command)); }
    std::vector<std::unique_ptr<Command>> getCommands() {return commands_; }
    void update(unsigned int frameTime);

    void tick();
};

#endif