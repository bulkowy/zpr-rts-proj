#include "Server.hpp"

#include <iostream>
#include <unistd.h>
#include <boost/algorithm/cxx14/equal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <src/networking/EventType.hpp>
#include <src/networking/Event.hpp>
#include <src/game/commands/CommandTypes.hpp>
#include <src/game/commands/Commands.hpp>

using namespace std;

namespace server
{

Server::Peer::Peer() : ready(false), timedOut(false) {
    socket.setBlocking(false);
}

Server::Server(int port) : 
    port_(port), 
    // gameThread_(&Server::run,this), 
    timeoutTime_(sf::seconds(30.f)),
    stop_(false),
    connectedPlayers_(0),
    maxPlayers_(5) {
    
    socketListener_.setBlocking(false);

    clients_.push_back(PeerPtr(new Peer()));
    //gameThread_.launch();
    // include random initialization and seed
}

Server::~Server() {
    stop_ = true;
    //gameThread_.wait();
}


void Server::run() {
    setListening(true);

    std::cout << "Server started running" << std::endl;

    sf::Thread gameLoop(&Server::gameLoop, this);
    gameLoop.launch();

	while (!stop_)
	{	
		handleIncomingPackets();
		handleConnections();

        tick();

		sf::sleep(sf::milliseconds(100));
	}	
}

void Server::tick() {

}

void Server::setListening(bool enable) {
    if (enable) {
        if (!isListening_) {
            isListening_ = (socketListener_.listen(port_) == sf::TcpListener::Done);
        } else {
            socketListener_.close();
            isListening_ = false;
        }
    }
}
void Server::handleIncomingPackets() {
    bool detectedTimeOut = false;

    for ( PeerPtr& peer : clients_ ) {
        if ( peer->ready ) {
            sf::Packet packet;
            while (peer->socket.receive(packet) == sf::Socket::Done) {
                handleIncomingPacket(packet, *peer, detectedTimeOut);

                peer->lastPacketTime = now();
                packet.clear();
            }

            if (now() >= peer->lastPacketTime + timeoutTime_) {
                peer->timedOut = true;
                detectedTimeOut = true;
            }
        }
    }

    if (detectedTimeOut) handleDisconnections();
}

void Server::handleIncomingPacket(sf::Packet& packet, Peer& peer, bool& detectedTimeout) {
    sf::Int32 type;
    packet >> type;

    std::cout << "test " << type << std::endl;

    switch (static_cast<networking::EventType>(type)) {
        case networking::EventType::Disconnected:
            peer.timedOut = true;
            detectedTimeout = true;
            break;
        case networking::EventType::Command:
            handleIncomingCommand(packet);
            break;
        default:
            break;
    }
}

void Server::handleIncomingCommand(sf::Packet& packet) {
    sf::Uint16 commandType;
    packet >> commandType;
    switch (static_cast<CommandType>(commandType))
    {
    case CommandType::MoveCommand:
        createCommand<MoveCommand>(packet);
        
        break;
    
    default:
        break;
    }
}
template<typename C>
void Server::createCommand(sf::Packet& packet) {
    std::unique_ptr<Command> command = std::make_unique<C>();
    packet >> *command;
    engine_.registerCommand(std::move(command));
}

void Server::handleConnections() {
    if (!isListening_) return;
    if (connectedPlayers_ >= maxPlayers_) return;


    if (socketListener_.accept(clients_[connectedPlayers_]->socket) == sf::TcpListener::Done) {
        sf::Packet packet;
        networking::Connected CONNevent;
        CONNevent.id_ = idseq_.get();

        std::cout << "Client connects at id " << CONNevent.id_ << std::endl;
        packet << CONNevent;
        clients_[connectedPlayers_]->socket.send(packet);
        packet.clear();

        informWorldState(clients_[connectedPlayers_]->socket);

        clients_[connectedPlayers_]->ready = true;
        sendToAll(packet);
        clients_[connectedPlayers_]->lastPacketTime = now();
        connectedPlayers_++;

        std::cout << "New Client Connected" << std::endl;

        if (connectedPlayers_ < maxPlayers_) clients_.push_back(PeerPtr(new Peer()));
        else setListening(false);
    }
}

void Server::handleDisconnections() {
    if (connectedPlayers_ == 0) return;
    networking::Disconnected event;
    sf::Packet packet;
    for ( auto peer = clients_.begin(); peer != clients_.end(); ) {
        if ( (*peer)->timedOut ) {
            connectedPlayers_--;
            peer = clients_.erase(peer);

            sendToAll(packet);
            packet.clear();

            std::cout << "Client Disconnected" << std::endl;
            if (connectedPlayers_ < maxPlayers_) {
                clients_.push_back(PeerPtr(new Peer()));
                if (!isListening_)
                    setListening(true);
            }
        } else {
            ++peer;
        }
    }
}

void Server::informWorldState(sf::TcpSocket& socket) {
    sf::Packet packet;

}

void Server::sendToAll(sf::Packet& packet) {
    for ( PeerPtr& peer : clients_ ) {
        if ( peer->ready ) peer->socket.send(packet);
    }
}

}