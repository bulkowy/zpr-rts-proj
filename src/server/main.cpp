#include <iostream>
#include "Server.hpp"
#include <src/networking/Event.hpp>
#include <src/client/Client.hpp>

int main () {
      server::Server server(4444);
      server.run();
    return 0;
}