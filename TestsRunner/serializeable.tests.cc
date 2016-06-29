#include "../Service/users.h"
#include "catch.hpp"

using namespace app;
using namespace utility;
using std::vector;

TEST_CASE("Entites are serialized and parsed correctly", "[serializeable]") {

	auto users = user::create_initial_users();

	SECTION("[card] serilaizes and parese correctly") {
		card crd = users[0].cards[0];
		auto value = serializeable<card>::serialize(crd);
		card parse_crd = serializeable<card>::parse(value);
		REQUIRE((crd == parse_crd));
	}

	SECTION("[transaction] serilaizes and parese correctly") {
		transaction tran = users[0].transactions[0];
		auto value = serializeable<transaction>::serialize(tran);
		transaction parse_tran = serializeable<transaction>::parse(value);

		REQUIRE(( tran == parse_tran ));
	}

	SECTION("[vector<T>] serilaizes and parese correctly") {
		vector<card> vec = users[0].cards;
		auto value = serializeable<vector<card>>::serialize(vec);
		auto parse_vec = serializeable<vector<card>>::parse(value);
		REQUIRE((vec == parse_vec));
	}

	SECTION("[user] serilaizes and parese correctly") {
		user usr = users[0];
		
		auto value = serializeable<user>::serialize(usr);
		auto parse_usr = serializeable<user>::parse(value);

		REQUIRE(( usr == parse_usr ));
	}
} 
