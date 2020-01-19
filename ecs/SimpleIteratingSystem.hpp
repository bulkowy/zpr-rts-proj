#ifndef _SIMPLEITERATINGSYSTEM_
#define _SIMPLEITERATINGSYSTEM_

#include "Engine.hpp"

namespace ecs {

class SimpleIteratingSystem : public System
{
private:
    
public:
    SimpleIteratingSystem(Engine& engine);
    void update(unsigned int elapsedTime);
    virtual void update(unsigned int elapsedTime, Entity entity) = 0;
};

SimpleIteratingSystem::SimpleIteratingSystem(Engine& engine)
    : System(engine)
{
}

/**
 * @brief Update wszystkich Obiektów.
 *
 * @param[in] frameTime  Czas, który upłynął od ostatniego wywołania, w milisekundach.
 */
 void SimpleIteratingSystem::update(unsigned int frameTime) {
    for (auto entity  = _entityFamilies[0].begin();
              entity != _entityFamilies[0].end();
            ++entity) {
        // Wywołaj update dla każdego Obiektu
        update(frameTime, *entity);
    }
    return;
}


} //namespace ecs

#endif