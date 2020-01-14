#include "Server.hpp"

#include <csignal>
#include <iostream>


namespace server
{

Server::Peer::Peer() : ready(false), timedOut(false) {
    socket.setBlocking(false);
}

Server::Server(int port) : 
    port_(port), 
    gameThread_(&Server::run,this), 
    timeoutTime_(sf::seconds(10.f)),
    stop_(false),
    connectedPlayers_(0),
    maxPlayers_(5),
    entitySequence_(0) {
    
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
    networking::UpdateEntity UEevent;
    Update update;
    sf::Packet packet;
    std::unordered_map<int, entity::Entity>::iterator it;
    while (!updateQueue_.empty()) {
        update = *(updateQueue_.front());
        updateQueue_.pop();
        it = entities_.find(update.entityID);
        it->second.setPosition(sf::Vector2f(update.newX, update.newY));
        UEevent.set(update.entityID, update.newX, update.newY);
        packet << UEevent;
        sendToAll(packet, update.entityID);
        packet.clear();
    }
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
    networking::UpdateEntity UEevent;

    std::cout << "test" << std::endl;

    switch (static_cast<networking::EventType>(type)) {
        case networking::EventType::Disconnected:
            peer.timedOut = true;
            detectedTimeout = true;
            break;
        case networking::EventType::UpdateEntity:
            std::cout << "testupdate" << std::endl;
            packet >> UEevent;
            if (peer.entityID == UEevent.id_)
                updateQueue_.push(UpdatePtr(new Update(UEevent.id_, UEevent.posX_, UEevent.posY_)));
            else {
                peer.timedOut = true;
                detectedTimeout = true;
            }
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

        CONNevent.id_ = entitySequence_;
        std::cout << "Client connects at id " << CONNevent.id_ << std::endl;
        packet << CONNevent;
        clients_[connectedPlayers_]->socket.send(packet);
        packet.clear();

        informWorldState(clients_[connectedPlayers_]->socket);

        entities_.insert({entitySequence_, entity::Entity(entitySequence_)});
        std::unordered_map<int, entity::Entity>::iterator it;
        it = entities_.find(entitySequence_);
        it->second.setPosition(sf::Vector2f(100.f, 100.f));
        networking::CreateEntity event;
        event.set(entitySequence_, 100.f, 100.f);
        packet << event;

        clients_[connectedPlayers_]->entityID = entitySequence_;
        clients_[connectedPlayers_]->ready = true;
        sendToAll(packet);
        clients_[connectedPlayers_]->lastPacketTime = now();
        connectedPlayers_++;
        entitySequence_++;

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
            entities_.erase((*peer)->entityID);
            event.id_ = (*peer)->entityID;
            packet << event;
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

    for( std::unordered_map<int, entity::Entity>::iterator it = entities_.begin(); it != entities_.end(); ++it ) {
        entity::Entity e = it->second;
        networking::CreateEntity event;
        event.set(e.entityID_, e.getPosition().x, e.getPosition().y);
        packet << event;
        socket.send(packet);
        packet.clear();
    } 
}

void Server::sendToAll(sf::Packet& packet) {
    for ( PeerPtr& peer : clients_ ) {
        if ( peer->ready ) peer->socket.send(packet);
    }
}

void Server::sendToAll(sf::Packet& packet, sf::Int32 entityID) {
    for ( PeerPtr& peer : clients_ ) {
        if ( peer->ready && peer->entityID != entityID) peer->socket.send(packet);
    }
}
}