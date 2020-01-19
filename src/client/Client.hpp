#ifndef __CLIENT_HPP_
#define __CLIENT_HPP_

#include <vector>
#include <memory>
#include <src/networking/Event.hpp>
#include <src/networking/EventType.hpp>
#include "ClientEngine.hpp"

namespace client
{
/**
 * @brief Class client that defines threads used for communication with main server
 * 
 * Client will talk either with main server or with server's game thread
 * when such thread will appear and Client will joing the game
 */
class Client
{
public:
    /**
     * @brief Base default constructor for Client class
     * 
     * Initializes main members for later usage
     */
    Client();

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    virtual ~Client();

    /**
     * @brief Main client loop
     * 
     * Defines loop that process packets from server and prepare ones from client to be sent
     */
    void run();

    /**
     * @brief Method used for stopping Client and its threads
     */
    void stop();

    /**
     * @brief Wait for all threads to end
     */
    void wait() { clientThread_.wait(); }

    /**
     * @brief Get the Client ID
     * 
     * @return int 
     */
    int getClientID() const { return clientID_; }

    /**
     * @brief Set the Client ID
     * 
     * @param clientID - new client ID
     */
    void setClientID(int clientID) { clientID_ = clientID; }

    // tmp?
    bool getStop() const { return stop_; }
    
    /**
     * @brief Send given packet to the Server
     * 
     * @param packet - packet to be sent to the server
     */
    void send(sf::Packet&);

    /**
     * @brief Secure method to disconnect from server
     */
    void disconnect();

    sf::TcpSocket socket_; /**< socket used in communication with server */

private:
    sf::Thread clientThread_; /**< thread for client's main communication loop */

    /**
     * @brief Method to parse updates on client and send them to server
     */
    void tick();

    /**
     * @brief Method deserializing incoming packet from server
     * 
     * @param packet - packet that came from server
     */
    void handleServerPacket(sf::Packet&);

    int clientID_;      /**< client's id */
    unsigned int port_; /**< port of server */
    bool stop_;         /**< is client stopped */
    bool isConnected_;  /**< is client connected */

    sf::Time clientTimeout_; /**< time for client to timeout from server */

    std::unique_ptr<ClientEngine> engine_;   /**< game engine */

};
}
#endif