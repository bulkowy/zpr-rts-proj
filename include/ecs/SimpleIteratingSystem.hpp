#ifndef _SIMPLEITERATINGSYSTEM_
#define _SIMPLEITERATINGSYSTEM_

#include "Engine.hpp"

namespace ecs {

class SimpleIteratingSystem : public System
{
private:
    
public:
    SimpleIteratingSystem(Engine& engine);
    void update(float elapsedTime);
    virtual void update(float elapsedTime, Entity entity);
};

SimpleIteratingSystem::SimpleIteratingSystem(Engine& engine)
    : System(engine)
{
}

/**
 * @brief Update wszystkich Obiektów.
 *
 * @param[in] elapsedTime  Czas, który upłynął od ostatniego wywołania, w sekundach.
 */
 void SimpleIteratingSystem::update(float elapsedTime) {
    for (auto entity  = _entityFamilies[0].begin();
              entity != _entityFamilies[0].end();
            ++entity) {
        // Wywołaj update dla każdego Obiektu
        update(elapsedTime, *entity);
    }
    return;
}


} //namespace ecs

#endif