#include <iostream>
#include "Event.hpp"
#include "Client.hpp"
#include "Entity.hpp"
#include "Server.hpp"

int main () {
    std::string run;
    std::cout << "Run as Server (s) or Client (c): ";
    std::cin >> run;
    if (run == "s") {
        server::Server server(4444);
        server.run();
    } else {
        client::Client client;
        std::string choice;
        while ( choice != "q" || !client.getStop() ) {
            std::cin >> choice;
            client.poolMove(std::stoi(choice));
        }
        client.wait();
    }
    return 0;
}