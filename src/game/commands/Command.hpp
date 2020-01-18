#ifndef _COMMAND_
#define _COMMAND_

#include <ecs/Engine.hpp>

class Command
{
public:
    Command() {}
    ~Command() {}

    virtual void execute(ecs::Engine& engine) = 0;
};


#endif