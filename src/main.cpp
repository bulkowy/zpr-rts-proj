#include <iostream>
#include <server/server.h>
#include <client/client.h>

using namespace std;
using namespace client;
using namespace server;

int main() {
    cout << "testing scons" << endl;
    i_am_the_client();
    i_am_the_server();
    return 0;
}
