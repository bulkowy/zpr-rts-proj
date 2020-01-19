#ifndef _GRIDMAP_
#define _GRIDMAP_

#include <boost/multi_array.hpp>

struct GridMapTile
{
    bool obstacle = false;
};


/**
 * Mapa kratkowa
 */
struct GridMap {
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 600;
    static const int TILE_SIDE = 40;
    static const int WIDTH = WINDOW_WIDTH/TILE_SIDE;
    static const int HEIGHT = WINDOW_HEIGHT/TILE_SIDE;

    boost::multi_array<GridMapTile, 2> map_;

    GridMap() {
        boost::array<int, 2> shape = { WIDTH, HEIGHT };
        boost::multi_array<GridMapTile, 2> map(shape);
        map_.resize(boost::extents[WIDTH][HEIGHT]);
        map_ = std::move(map);
    }
};


#endif