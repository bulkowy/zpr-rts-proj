#ifndef _MOVE_
#define _MOVE_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>

/**
 * Komponent odpowiadający za dane związane z ruchem danego Obiektu
 */
struct Move : public ecs::Component {
    /**
     * Indeks kratki, do której przemieszcza się teraz dany obiekt
     */
    struct Position {
        int x;
        int y;
    } destination;
    
    /**
     * Prędkość z jaką przemieszcza się jednostka, liczona w kratkach na sekundę
     */
    float speed;

    static const ecs::ComponentType _type;
};


#endif