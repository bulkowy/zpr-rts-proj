#include "Client.hpp"
#include <iostream>
#include <src/client/ClientEngine.hpp>

namespace client {

Client::Client() :
    clientThread_(&Client::run, this),
    clientID_(-1),
    stop_(false),
    clientTimeout_(sf::seconds(999.0f))
    {

    if (socket_.connect("127.0.0.1", 4444, sf::seconds(5.f)) == sf::TcpSocket::Done) 
        isConnected_ = true;

    socket_.setBlocking(false);
    clientThread_.launch();
}

Client::~Client() { stop_ = true; clientThread_.wait(); }

void Client::run() {
    std::cout << "Client started running" << std::endl;
    sf::Packet packet;

    sf::Clock noPacketClock;
    sf::Time noPacketTime = sf::Time::Zero;

    engine_ = std::make_unique<ClientEngine>();

    sf::Thread gameLoop(&Client::gameLoop, this);
    gameLoop.launch();

	while (!stop_)
	{
        if (noPacketTime > clientTimeout_) { stop(); break; }
        if (socket_.receive(packet) == sf::Socket::Done) {
		    handleServerPacket(packet);
            noPacketTime = sf::Time::Zero;
        } else {
            noPacketTime += noPacketClock.getElapsedTime();
        }
        noPacketClock.restart();

		stop_ = tick();

		sf::sleep(sf::milliseconds(100));
	}
    gameLoop.wait();
}

bool Client::tick() {
    sendCommands();
    return !engine_->getWindow()->isOpen();
}

void Client::sendCommands() {
    sf::Packet packet;
    for (auto &&command : engine_->getCommands())
    {
        packet.clear();
        packet << *command;
        socket_.send(packet);
    }

    engine_->getCommands().clear();
    
}

void Client::handleServerPacket(sf::Packet& packet) {
    sf::Int32 type;
    packet >> type;

    switch (static_cast<networking::EventType>(type)) {
        case networking::EventType::Connected:
        {
            sf::Int32 id;
            packet >> id;
            std::cout << "Connected to the server with ID: " << id << std::endl;
            sf::sleep(sf::milliseconds(500));
        }   break;

        case networking::EventType::Disconnected:
        {
            std::cout << "Żegnaj :c " << std::endl;
            
        } break;

        case networking::EventType::EntitySet:
        {
            sf::Uint32 size, ent;
            packet >> size;
            std::set<ecs::Entity> set;
            for(sf::Uint32 i = 0; i<size; ++i) {
                packet >> ent;
                set.insert(ecs::Entity(ent));
            }
            engine_->registerEntitySet(set);
        } break;
        
        default:
        {

        } break;
    }
}

void Client::stop() {
    stop_ = true;
    isConnected_ = false;
    std::cout << "Client " << clientID_ << " stopped running";
}

void Client::disconnect() {
    socket_.disconnect();
    stop();
}


}