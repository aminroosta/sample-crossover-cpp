#include "../Service/users.h"
#include "catch.hpp"

using namespace app;
using namespace utility;
using std::vector;

TEST_CASE("Can create dummy users", "[users]") {

	vector<user> users;
	
	REQUIRE_NOTHROW((users = user::create_initial_users()));
	REQUIRE(users.size() > 0);

	SECTION("We have a card for each user") {
		bool ok = true;
		for (auto& u : users)
			if (u.cards.empty())
				ok = false;
		REQUIRE(ok);
	}
}
