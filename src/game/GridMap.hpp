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
    static const int TILE_SIDE = 40;

    boost::multi_array<GridMapTile, 2> _map;

    GridMap(int width, int height) {
        boost::array<int, 2> shape = { width, height };
        boost::multi_array<GridMapTile, 2> map(shape);
        _map.resize(boost::extents[width][height]);
        _map = std::move(map);
    }
};


#endif