#include "Game.hpp"
#include "Client.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

namespace game
{
int Game::gamesCount_ = 0;

Game::Game(const std::string& mapFileName) :
    isRunning_(false),
    gameThread_(&Game::run_,this),
    sendThread_(&Game::send_,this),
    gameID_(++gamesCount_) {

    minCoord_.x = 0;
    minCoord_.y = 0;
    maxCoord_.x = 100;
    maxCoord_.y = 100;
}

Game::~Game() {
    for(client::Client* c: clients_)
        delete c;
}

bool Game::addClient(client::Client* client) {
    sf::Packet response;
    response << networking::JoinGame(gameID_);
    client->send(response);

    return true;
}


void Game::run() {
    isRunning_ = true;
    gameThread_.launch();
    sendThread_.launch();
}

void Game::stop() {
    isRunning_ = false;
}
/*
Entity& Game::createEntity(const sf::Vector2i& coord,Team* team,MakeAs makeAs)
{
    std::uint32_t id = entities.create();
    Entity& e = entities.get(id);

    makeAs(e,team,*this);

    e.setPosition(_map->mapCoordsToPixel(coord),coord);
    _byCoords[coord].emplace_back(&e);

    addCreate(_createEntities,id);
    return e;
}


void Game::setPosition(Entity& e,const sf::Vector2i& oldCoord,const sf::Vector2f& oldPos,const sf::Vector2i& newCoord, const sf::Vector2f& newPos)
{
    markEntityUpdated(e.id());
    e.setPosition(newPos,newCoord);

    if(oldCoord != newCoord)
    {
        _byCoords[oldCoord].remove(&e);
        _byCoords[newCoord].emplace_back(&e);
    }
}
*/

void Game::run_()
{
    sf::Clock clock;        
    sf::Time timeSinceLastUpdate;

    sf::Time TimePerFrame = sf::seconds(1.f/120);

    while(isRunning_) {
        sf::Lock gameGuard(gameMutex_);
        processNetworkEvents();

        sf::Time delta = clock.restart();
        timeSinceLastUpdate += delta;
        if(timeSinceLastUpdate > TimePerFrame) {
            timeSinceLastUpdate -= TimePerFrame;
            update(TimePerFrame);
        }
    }
}

void Game::send_(){
    while(isRunning_) {
        sendMutex_.lock();
        if(outgoing_.size() > 0) {
            sf::Packet packet = outgoing_.front();
            outgoing_.pop();
            sendMutex_.unlock();

            sf::Lock guard(clientsMutex_);
            for(auto it = clients_.begin(); it != clients_.end();++it) {
                (*it)->send(packet);
            }
        } else {
            sendMutex_.unlock();
        }
    }
}

void Game::processNetworkEvents() {
    sf::Lock guard(clientsMutex_);
    for(auto it = clients_.begin(); it != clients_.end();++it) {
        client::Client* client = *it;
        networking::Event* event;
        while(client and client->pollEvent(event)) {
            switch(event->getType())
            {
                case networking::EventType::Disconnected :
                    it = clients_.erase(it);
                    --it;
                    delete client;
                    client = nullptr;
                    break;
                default: 
                    break;
            }
        }
    }
}
void Game::sendUpdates() {
    if(updateEntitiesId_.size() > 0)
    {
        networking::UpdateEntity update;

        for(std::uint32_t id : updateEntitiesId_)
            addUpdate(update,id);

        sf::Packet packet;
        packet << update;
        sendToAll(packet);

        updateEntitiesId_.clear();
    }
}

void Game::update(sf::Time deltaTime) {
    sendUpdates();
}

void Game::addUpdate(networking::UpdateEntity& packet, sf::Uint32 id) {
    entity::Entity* e = findEntity_(id);
    if(e) {
        networking::UpdateEntity::Data update;

        update.entityId = id;
        update.position = e->getPosition();

        packet.add(std::move(update));
    }
}

void Game::sendToAll(sf::Packet& packet) {
    sf::Lock guard(sendMutex_);
    outgoing_.emplace(packet);
}

entity::Entity* Game::findEntity_(sf::Uint32 id) {
    std::unordered_map<int, entity::Entity>::iterator it;
    it = entities_.find(id);
    if (it == entities_.end()) return nullptr;
    return &(it->second);
}

}