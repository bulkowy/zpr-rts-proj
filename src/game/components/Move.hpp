#ifndef _MOVE_
#define _MOVE_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>
#include <SFML/Network.hpp>

/**
 * Komponent odpowiadający za dane związane z ruchem danego Obiektu
 */
struct Move : public ecs::Component {
    /**
     * Indeks kratki, do której przemieszcza się teraz dany obiekt
     */
    struct Position {
        int x = 0;
        int y = 0;
    } destination;
    
    /**
     * Prędkość z jaką przemieszcza się jednostka, liczona w kratkach na sekundę
     */
    float speed;

    void inline setDestination(int x, int y) {destination.x = x; destination.y = y;}
    void inline setSpeed(float newSpeed) {speed = newSpeed;}

    Move() {}
    Move(float aSpeed) : speed(aSpeed) {}

    static const ecs::ComponentType _type;

    friend sf::Packet& operator>>(sf::Packet& packet, Move& move);
    friend sf::Packet& operator<<(sf::Packet& packet, Move& move);

    sf::Packet& serialize(sf::Packet&);
    sf::Packet& deserialize(sf::Packet&);

};

#endif