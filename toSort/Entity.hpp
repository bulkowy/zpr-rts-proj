#ifndef ENTITY_HPP_
#define ENTITY_HPP_

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

namespace entity
{

class Entity
{
public:
    Entity(sf::Int32 entityID) : entityID_(entityID) { ++entityNum_; }

    void setPosition(sf::Vector2f pos) {
        pos_.x = pos.x;
        pos_.y = pos.y;
    }

    sf::Vector2f getPosition() {
        return pos_;
    }
    int entityID_;
    sf::Vector2f pos_;

private:
    static int entityNum_;
};

}

#endif