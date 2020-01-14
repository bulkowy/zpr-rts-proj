#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <vector>
#include "Event.hpp"
#include "EventType.hpp"
#include "Entity.hpp"

namespace client
{
class Client
{
public:
    Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    virtual ~Client();

    void run();
    void stop();
    void wait() { clientThread_.wait(); }

    int getClientID() const { return clientID_; }
    int setClientID(int clientID) { clientID_ = clientID; }

    bool getStop() const { return stop_; }
    
    void send(sf::Packet&);
    void poolMove(int move);

    void disconnect();

    sf::TcpSocket socket_;

private:
    typedef std::unique_ptr<entity::Entity> EntityPtr; 
    sf::Thread clientThread_;

    void tick();
    void handleServerPacket(sf::Packet&);

    int clientID_;
    unsigned int port_;
    bool stop_;
    bool isConnected_;

    enum NextMove {
        NOMOVE, UP, DOWN, LEFT, RIGHT
    };

    NextMove nm_;

    sf::Int32 ownEntityID_;
    std::vector<entity::Entity> allEntities_;
    sf::Time clientTimeout_;

};
}
#endif