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
    GridMap map_;
    std::vector<std::unique_ptr<Command>> commands_;

    void executeCommands();

public:
    ServerEngine();
    ~ServerEngine() {}
    GridMap* getGridMap() { return &map_; }
    void registerCommand(std::unique_ptr<Command> command) {commands_.push_back(std::move(command)); }

    inline void update(unsigned int frameTime) override;
};

#endif