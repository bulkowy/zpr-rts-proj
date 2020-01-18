#ifndef _SELECTABLE_
#define _SELECTABLE_

#include <ecs/Component.hpp>
#include <ecs/ComponentType.hpp>
#include <memory>

/**
 * Komponent określający, czy dany Obiekt został zaznaczony
 */
struct Selectable : public ecs::Component {

    /** Znacznik do wyświetlenia
     */
    std::unique_ptr<sf::Sprite> selectedSprite;

    /**
     * Znacznik destynacji do wyświetlenia
     */
    std::unique_ptr<sf::Sprite> destinationSprite;

    Selectable(sf::Sprite* aSprite) : selectedSprite(aSprite) { destinationSprite = std::unique_ptr<sf::Sprite>(FileManager::getInstance()->getSprite("destination")); }

    bool selected = false;

    static const ecs::ComponentType _type;
};


#endif