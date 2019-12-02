#include "../../client/client.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(ClientTest)

BOOST_AUTO_TEST_CASE(client_test) {
    BOOST_CHECK_NO_THROW(client::i_am_the_client());
}

BOOST_AUTO_TEST_SUITE_END()