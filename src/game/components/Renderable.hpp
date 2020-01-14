#ifndef _RENDERABLE_
#define _RENDERABLE_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

/**
 * Komponent zawierający dane potrzebne do wyświetlania Obiektu
 */
struct Renderable : public ecs::Component {
    /** Obrazek do wyświetlenia
     */
    std::unique_ptr<sf::Sprite> sprite;

    Renderable(sf::Sprite* aSprite) : sprite(aSprite) {}
    
    static const ecs::ComponentType _type;
};


#endif