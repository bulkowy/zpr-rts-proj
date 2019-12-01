#include "../server/server.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ServerTests)

BOOST_AUTO_TEST_CASE(server_test) {
    BOOST_CHECK_EQUAL(server::i_am_the_server(), true);
}

BOOST_AUTO_TEST_SUITE_END()