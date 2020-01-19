#include "Client.hpp"
#include <iostream>


namespace client {

Client::Client() :
    clientThread_(&Client::run, this),
    clientID_(-1),
    stop_(false),
    nm_(NextMove::NOMOVE),
    clientTimeout_(sf::seconds(999.0f))
    {

    if (socket_.connect("127.0.0.1", 4444, sf::seconds(5.f)) == sf::TcpSocket::Done) 
        isConnected_ = true;

    socket_.setBlocking(false);
    clientThread_.launch();
}

Client::~Client() { stop_ = true; clientThread_.wait(); }

void Client::poolMove(int move) {
    if (move >= 0 && move <= 4) {
        nm_ = static_cast<Client::NextMove>(move);
    }
}

void Client::run() {
    std::cout << "Client started running" << std::endl;
    sf::Packet packet;

    sf::Clock noPacketClock;
    sf::Time noPacketTime = sf::Time::Zero;

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

        if (ownEntityID_ < 0) {
		    sf::sleep(sf::milliseconds(100));
            continue;
        }

		tick();

		sf::sleep(sf::milliseconds(100));
	}	
}

void Client::tick() {
    sf::Vector2f vec(-1.f, -1.f);
    int idx = -1;
    for (unsigned int i = 0; i < allEntities_.size(); i++) {

        if (allEntities_[i].entityID_ == ownEntityID_) {
            vec = allEntities_[i].pos_;
            idx = i;
            break;
        }
    }
    if (idx < 0) return;
    if (vec.x < 0) return;
    networking::UpdateEntity event;
    sf::Packet packet;
    switch (nm_) {
        case NextMove::NOMOVE:
            return;
        case NextMove::UP:
            ++vec.y; break;
        case NextMove::DOWN:
            --vec.y; break;
        case NextMove::RIGHT:
            ++vec.x; break;
        case NextMove::LEFT:
            --vec.x; break;
    }
    
    allEntities_[idx].setPosition(vec);
    std::cout << idx << vec.x << vec.y << std::endl;
    event.set(ownEntityID_, vec.x, vec.y);
    packet << event;
    socket_.send(packet); 
    nm_ = NextMove::NOMOVE;
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
            ownEntityID_ = id;
            sf::sleep(sf::milliseconds(500));
        }   break;

        case networking::EventType::Disconnected:
        {
            sf::Int32 entityID;
            packet >> entityID;

            std::cout << "Disconnected ID: " << entityID << std::endl;
            
            for ( auto e = allEntities_.begin(); e != allEntities_.end(); ) {
                if ( (*e).entityID_ == entityID )
                    allEntities_.erase(e);
                else 
                    ++e;
            }
        } break;

        case networking::EventType::CreateEntity:
        {
            sf::Int32 entityID;
            float posX, posY;
            packet >> entityID >> posX >> posY;
            entity::Entity e(entityID);
            e.setPosition(sf::Vector2f(posX, posY));
            allEntities_.push_back(e);
            std::cout << "Added new entity ID " << entityID << " at x:y " << posX << ":" << posY << std::endl;
        } break;

        case networking::EventType::UpdateEntity:
        {
            sf::Int32 entityID;
            float posX, posY;
            packet >> entityID >> posX >> posY;
            allEntities_[entityID].setPosition(sf::Vector2f(posX, posY));
            std::cout << "Updated entity ID " << entityID << " at x:y " << posX << ":" << posY << std::endl;
        } break;
        
        default:
        {

        } break;
    }
}

void Client::stop() {
    stop_ = true;
    isConnected_ = false;
    std::cout << "Client " << ownEntityID_ << " stopped running";
}

void Client::disconnect() {
    socket_.disconnect();
    stop();
}


}