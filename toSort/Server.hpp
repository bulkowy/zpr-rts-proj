#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <queue>
#include <unordered_map>
#include "Client.hpp"
//#include "Game.hpp"
#include "EventType.hpp"
#include "Entity.hpp"


namespace server
{

class Server
{
public:
    Server(int port);

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    ~Server();

    void run();

private:

    struct Peer {
        Peer();
        sf::TcpSocket socket;
        sf::Time lastPacketTime;
        sf::Int32 entityID;
        bool ready;
        bool timedOut;
    };

    struct Update {
        Update() : entityID(-1), newX(0), newY(0) {}
        Update(sf::Int32 id, float x, float y) : entityID(id), newX(x), newY(y) {} 
        sf::Int32 entityID;
        float newX;
        float newY;
    };

    typedef std::unique_ptr<Peer> PeerPtr;
    typedef std::unique_ptr<Update> UpdatePtr;

    const unsigned int port_;
    sf::Clock clock_;

    void setListening(bool);
    void handleIncomingPackets();
    void handleIncomingPacket(sf::Packet&, Peer&, bool&);
    void handleConnections();
    void handleDisconnections();
    void sendToAll(sf::Packet&);
    void sendToAll(sf::Packet&, sf::Int32);
    void tick();
    void informWorldState(sf::TcpSocket&);

    sf::Time now() const {
        return clock_.getElapsedTime();
    }

    sf::Thread gameThread_;
    sf::Mutex gameMutex_;
    //std::vector<game::Game*> games_;
    sf::Mutex clientMutex_;
    std::vector<PeerPtr> clients_;

    sf::TcpListener socketListener_;
    sf::Time timeoutTime_;
    bool isListening_;
    bool stop_;
    int connectedPlayers_;
    int maxPlayers_;
    int entitySequence_;

    std::unordered_map<int, entity::Entity> entities_;
    std::queue<UpdatePtr> updateQueue_;
};

}
#endif