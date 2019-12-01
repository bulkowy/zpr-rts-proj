#include "server.h"

#include <iostream>
#include <boost/algorithm/cxx14/equal.hpp>

using namespace std;

void server::i_am_the_server() {
    int a[] = { 1, 2 };
    int b[] = { 1, 2 };

    bool ans = boost::algorithm::equal(a, a + 2, b, b + 2);
    cout << "answer is " << ans << endl;
}
