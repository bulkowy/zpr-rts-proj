#ifndef GAME_HPP_
#define GAME_HPP_

#include <vector>
#include <string>
#include <queue>
#include <set>
#include <unordered_map>
#include <functional>

#include "Client.hpp"
#include "Entity.hpp"

namespace game
{

class Game
{
public:
    Game(const std::string& mapFileName);

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    ~Game();

    int getPlayersCount() {
        sf::Lock guard(clientsMutex_);
        return clients_.size();
    }

    int getGameID() const { return gameID_; }

    bool addClient(client::Client* client);

    void run();
    void stop();

    //Entity& createEntity(const sf::Vector2i& coord,Team* team,MakeAs makeAs);
    //void destroyEntity(std::uint32_t id);

    //void setPosition(Entity& e,const sf::Vector2i& oldCoord,const sf::Vector2f& oldPos,const sf::Vector2i& newCoord,const sf::Vector2f& newPos);

    using FuncType = std::function<void(client::Client*)>;
    FuncType onLogOut;

private:
    bool isRunning_;
    sf::Thread gameThread_;
    sf::Mutex gameMutex_;

    std::set<std::uint32_t> updateEntitiesId_;
    
    sf::Vector2f minCoord_;
    sf::Vector2f maxCoord_;

    sf::Mutex clientsMutex_;
    std::vector<client::Client*> clients_;

    void send_();
    sf::Thread sendThread_;
    sf::Mutex sendMutex_;
    std::queue<sf::Packet> outgoing_;

    std::string mapName_;


    const int gameID_;
    static int gamesCount_;

    void run_();

    void processNetworkEvents();
    void sendUpdates();
    void update(sf::Time deltaTime);

    void addUpdate(networking::UpdateEntity& packet, sf::Uint32 id);

    void sendToAll(sf::Packet& packet);

    // change to entitymanager
    std::unordered_map<int, entity::Entity> entities_;

    entity::Entity* findEntity_(sf::Uint32 id);
};

}
#endif