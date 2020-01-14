#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include <vector>
#include <queue>
#include <unordered_map>
#include "Client.hpp"
#include "Game.hpp"
#include "EventType.hpp"
#include "Entity.hpp"


namespace server
{
/**
 * @brief Server class that defines main thread of game server logic. 
 * 
 * It will be responsible for creating and stopping new games, assigning clients to them,
 * informing about all games that are currently running or open. 
 * 
 * For every game, it will attempt to create new Game thread. 
 */
class Server
{
public:
    /**
     * @brief Base parameter constructor for Server class. 
     * 
     * Initializes main members that will be required in work of the server. 
     * 
     * @param port - port set for main connection listener. 
     */
    Server(int port);

    Server(const Server&) = delete;
    Server& operator=(const Server&) = delete;

    ~Server();

    /**
     * @brief Main server loop
     * 
     * Defines loop of server that allows it to run connection management. 
     */
    void run();

private:

    /**
     * @brief Struct defining Client connection to the server
     */
    struct Peer {
        /**
         * @brief Construct a new Peer object
         * 
         * Initializes bool values and sets socket as nonblocking. 
         */
        Peer();
        sf::TcpSocket socket;       /**< socket assigned for communication with Client */
        sf::Time lastPacketTime;    /**< timestamp of time when last packet from Client was received */
        sf::Int32 entityID;     // todo bo to zniknie
        bool ready;                 /**< bool value defining Client's readiness to accept Server packets */
        bool timedOut;              /**< bool value defining whether Client got disconnected */
    };

    /**
     * @brief Struct defining Update packet of Entity
     * 
     * Used for reacting at changes from client and sending them further to other ones. 
     */
    struct Update {
        /**
         * @brief Construct a new Update object
         * 
         * Initializes values with illegal values as it defines Update packet as not complete. 
         */
        Update() : entityID(-1), newX(0), newY(0) {}

        /**
         * @brief Construct a new Update object
         * 
         * Given values in parameters, try to create Update object that will be completed and ready to process. 
         * 
         * @param id    ID of Client
         * @param x     new X coord
         * @param y     new Y coord
         */
        Update(sf::Int32 id, float x, float y) : entityID(id), newX(x), newY(y) {} 

        sf::Int32 entityID; // todo tmp
        float newX;
        float newY;
    };

    typedef std::unique_ptr<Peer> PeerPtr;
    typedef std::unique_ptr<Update> UpdatePtr;

    const unsigned int port_;       /**< port used for Server's main connection listener */
    sf::Clock clock_;               /**< clock defining how long is server running */

    /**
     * @brief Method used for opening or closing listener
     * 
     * @param enable - if true, then start listening, otherwise stop
     */
    void setListening(bool);

    /**
     * @brief Method handling received messages from all clients
     * 
     * After receiving message, function passes execution to `handleIncomingPacket`
     */
    void handleIncomingPackets();

    /**
     * @brief Method handling reveived Event packet from selected client
     * 
     * Runs after call from upper function `handleIncomingPackets`
     */
    void handleIncomingPacket(sf::Packet&, Peer&, bool&);

    /**
     * @brief Handle connections and create every necessary object to sustain connection with new Client
     */
    void handleConnections();

    /**
     * @brief Handle disconnections and delete every object assigned to dropped Client
     */
    void handleDisconnections();

    /**
     * @brief Send given Packet to every Client connected to Server
     * 
     * @param packet - packet processed and ready to be sent
     */
    void sendToAll(sf::Packet&);

    /**
     * @brief Send given Packet to every Client EXCEPT the one given in param connected to Server
     * 
     * @param packet - packet processed and ready to be sent
     * @param entityID - Client's ID        todozmiananazwy 
     */
    void sendToAll(sf::Packet&, sf::Int32);

    /**
     * @brief Function for processing all Updates that came from Clients 
     */
    void tick();

    /**
     * @brief Send all information about world to selected socket
     * 
     * @param socket - socket that will receive world's information
     */
    void informWorldState(sf::TcpSocket&);

    /**
     * @brief Return for how long server runs
     * 
     * @return sf::Time server's execution time
     */
    sf::Time now() const {
        return clock_.getElapsedTime();
    }

    std::vector<PeerPtr> clients_;      /**< clients connected to server */

    sf::TcpListener socketListener_;    /**< main server connection listener */
    sf::Time timeoutTime_;              /**< time defining how long silence from Client will be tolerated */
    bool isListening_;                  /**< bool value defining whether listener is active */
    bool stop_;                         /**< bool value defining whether it's time for server to stop */
    int connectedPlayers_;              /**< current amount of players connected to server */
    int maxPlayers_;                    /**< maximum possible players connected to server */
    int maxGames_;                      /**< maximum possible games created by server and simultaneously runned */
    int entitySequence_;                /**< sequence of ID's for entities / clients */

    std::vector<game::Game*> games_;    /**< vector of currently running or awaiting games */

    std::unordered_map<int, entity::Entity> entities_;  /**< vector of currently existing entities */
    std::queue<UpdatePtr> updateQueue_;                 /**< queue of updates from Clients */
};

}
#endif