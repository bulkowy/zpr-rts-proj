#ifndef _POSITION_
#define _POSITION_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>

/**
 * Komponent określający pozycję danego Obiektu na mapie kratkowej
 */
struct Position : public ecs::Component {
    /** Numer kratki w poziomie
     */
    int x = 0;
    /** Numer kratki w pionie
     */
    int y = 0;
    /** Procent drogi do sąsiedniej kratki w poziomie, jaki wykonał obiekt (od -100 do 100)
     */
    int xoffset = 0;
    /** Procent drogi do sąsiedniej kratki w pionie, jaki wykonał obiekt (od -100 do 100)
     */
    int yoffset = 0;

    Position() {};
    Position(int aX, int aY) : x(aX), y(aY) {};

    static const ecs::ComponentType _type;
};


#endif