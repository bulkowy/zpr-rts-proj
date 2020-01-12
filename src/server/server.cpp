#include "server.h"
#include "../../include/ecs/Engine.hpp"

#include <iostream>
#include <boost/algorithm/cxx14/equal.hpp>

using namespace std;

bool server::i_am_the_server() {
    ecs::Engine engine;
    ecs::Entity entity1 = engine.createEntity();

    cout << "Entity1: " << entity1 << endl;

    int a[] = { 1, 2 };
    int b[] = { 1, 2 };

    bool ans = boost::algorithm::equal(a, a + 2, b, b + 2);
    cout << "answer is " << ans << endl;
    return ans;
}
