#include "../Service/authorize.h"
#include "catch.hpp"

using namespace app;
using namespace utility;
using namespace pplx;

TEST_CASE("authorize is working as expected", "[authorize]") {

	auto users = repository::instance().get_users().get();
	authorize& auth = authorize::instance();

	/* wrong user pass fails */
	auto token = auth.login(users[0].name, U("wrong-password!")).get();
	REQUIRE(token.empty());

	/* logins successfully */
	token = auth.login(users[0].name, users[0].password).get();
	REQUIRE(!token.empty());

	/* can set the card when registered */
	bool ok = auth.set_card(token, users[0].cards[0].id, users[0].cards[0].pin).get();
	REQUIRE(ok);

	/* still logged in */
	ok = auth.validate_token(token);
	REQUIRE(ok);

	/* should logout after the timeout */
	auth.timeout = std::chrono::milliseconds(10);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	ok = auth.validate_token(token);
	REQUIRE(!ok);

}
