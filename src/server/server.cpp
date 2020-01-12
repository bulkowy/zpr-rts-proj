#include "server.h"

#include <iostream>
#include <unistd.h>
#include <boost/algorithm/cxx14/equal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "../../include/ecs/Engine.hpp"
#include "../game/FileManager.hpp"

using namespace std;

bool server::i_am_the_server() {
    ecs::Engine engine;

    ecs::Entity entity1 = engine.createEntity();
    cout << "Entity1: " << entity1 << endl;

    FileManager fileManager = FileManager();
    fileManager.init("resources/sprites.json");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Test!");
	window.setKeyRepeatEnabled(false);

    sf::Sprite* sprites[] = {fileManager.getSprite("misiek"), fileManager.getSprite("tubaman"), fileManager.getSprite("grass"), fileManager.getSprite("tree"), fileManager.getSprite("toolbar")};
    for (size_t i = 0; i < 5; i++)
    {
        sprites[i]->setPosition(100.0f*i, 300.0f);
        window.draw(*sprites[i]);
    }
    window.display();

    sleep(5);

    int a[] = { 1, 2 };
    int b[] = { 1, 2 };

    bool ans = boost::algorithm::equal(a, a + 2, b, b + 2);
    cout << "answer is " << ans << endl;
    return ans;
}
