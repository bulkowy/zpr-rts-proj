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

    std::set<ecs::Entity> temp;
    temp.insert(engine_->createEntity());
    temp.insert(engine_->createEntity());
    networking::MoveCommand moveCommand(temp, 42, 56);
    packet << moveCommand;
    socket_.send(packet);
    packet.clear();
}

bool Client::tick() {
    return engine_->tick();
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