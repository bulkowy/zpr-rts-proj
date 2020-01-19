#ifndef _MAPRENDERER_
#define _MAPRENDERER_

#include <boost/multi_array.hpp>
#include <algorithm>

#include <SFML/Graphics.hpp>
#include <src/game/GridMap.hpp>
#include <src/game/FileManager.hpp>
#include <ecs/System.hpp>
#include <ecs/Engine.hpp>

/**
 * System renderujący podstawowe tło (trawę) mapy kratkowej
 */
class MapRenderer : public ecs::System {
private:
    boost::multi_array<sf::Sprite*, 2> map_;
public:
    MapRenderer(ClientEngine &engine) : ecs::System(engine) {
        // ServerEngine* engine = dynamic_cast<ServerEngine*>(&_engine);
        auto map(engine.getGridMap()->map_);

        boost::array<boost::multi_array_types::size_type, 2> shape = { map.shape()[0], map.shape()[1] };
        boost::multi_array<sf::Sprite*, 2> temp(shape);
        map_.resize(boost::extents[shape[0]][shape[1]]);
        map_ = std::move(temp);

        std::for_each(map_.origin(), map_.origin() + map_.num_elements(), createGrass);

        for(unsigned int i=0; i<map_.size(); ++i) {
            for(unsigned int j=0; j<map_[i].size(); ++j) {
                map_[i][j]->setPosition(i*GridMap::TILE_SIDE, j*GridMap::TILE_SIDE);
            }
        }
    }

    static void createGrass(sf::Sprite* &p) {
    p = FileManager::getInstance()->getSprite("grass");
    }


    void update(unsigned int a) {
        ClientEngine* engine = dynamic_cast<ClientEngine*>(&_engine);
        auto f = [&](sf::Sprite* s) {
            (engine->getWindow())->draw(*s);
        };
        std::for_each(map_.origin(), map_.origin()+map_.num_elements(), f);
    }
};



#endif