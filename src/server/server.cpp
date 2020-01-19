#include "Server.hpp"

#include <iostream>
#include <unistd.h>
#include <boost/algorithm/cxx14/equal.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "ServerEngine.hpp"
#include <src/game/FileManager.hpp>
#include <src/game/components/Components.hpp>
#include <src/client/BasicRenderer.hpp>
#include <src/networking/EventType.hpp>
#include <src/networking/Event.hpp>
#include "systems/Systems.hpp"

// do clienta
#include <src/client/MapRenderer.hpp>
#include <src/client/BasicRenderer.hpp>
#include <src/client/SelectedRenderer.hpp>
#include <src/client/DestinationRenderer.hpp>

using namespace std;

bool server::i_am_the_server() {
    static const int WIDTH = 800;
    static const int HEIGHT = 600;
    ServerEngine engine(WIDTH/GridMap::TILE_SIDE, HEIGHT/GridMap::TILE_SIDE);

    std::shared_ptr<sf::RenderWindow> window (new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Test!"));
	window->setKeyRepeatEnabled(false);
    engine.setWindow(window);

    FileManager* fileManager = FileManager::getInstance();
    fileManager->init("resources/sprites.json");

    engine.createComponentStore<Position>();
    engine.createComponentStore<Move>();
    engine.createComponentStore<Health>();
    engine.createComponentStore<Renderable>();
    engine.createComponentStore<Selectable>();

    engine.addSystem(ecs::System::Ptr(new MoveSystem(engine)));
    engine.addSystem(ecs::System::Ptr(new MapRenderer(engine)));
    engine.addSystem(ecs::System::Ptr(new DestinationRenderer(engine)));
    engine.addSystem(ecs::System::Ptr(new BasicRenderer(engine)));
    engine.addSystem(ecs::System::Ptr(new SelectedRenderer(engine)));


    ecs::Entity entity1 = engine.createEntity();
    cout << "Entity1: " << entity1 << endl;

    engine.addComponent<Position>(entity1, Position());
    Renderable renderable(fileManager->getSprite("misiek"));
    engine.addComponent<Renderable>(entity1, std::move(renderable));
    engine.addComponent<Health>(entity1, Health(100));
    Move move(2);
    move.setDestination(0, 5);
    engine.addComponent<Move>(entity1, std::move(move));
    engine.addComponent<Selectable>(entity1, Selectable(fileManager->getSprite("selected")));

    engine.registerEntity(entity1);
    
    while(window->isOpen()){
        sf::Event event;
        while (window->pollEvent(event)) { engine.handleEvent(event); }
        window->clear(sf::Color::Black);
        engine.update(17);
        window->display();
    }

    return true;
}

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

    std::cout << "test" << std::endl;

    switch (static_cast<networking::EventType>(type)) {
        case networking::EventType::Disconnected:
            peer.timedOut = true;
            detectedTimeout = true;
            break;
    
        default:
            break;
    }
}

void Server::handleConnections() {
    if (!isListening_) return;
    if (connectedPlayers_ >= maxPlayers_) return;


    if (socketListener_.accept(clients_[connectedPlayers_]->socket) == sf::TcpListener::Done) {
        sf::Packet packet;
        networking::Connected CONNevent;

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