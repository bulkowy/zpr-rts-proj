#ifndef _HEALTH_
#define _HEALTH_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>

/**
 * Komponent odpowiadający za liczbę punktów życia jednostki lub budynku
 */
struct Health : public ecs::Component {
    unsigned int max;
    unsigned int amount;

    Health(int arg) : max(arg) {amount = max;}

    static const ecs::ComponentType _type;
};


#endif