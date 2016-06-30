#include "../Service/repository.h"
#include <iostream>
#include <string>
#include <stdexcept>

#include "catch.hpp"

using namespace app;
using namespace concurrency::streams;
using namespace utility;
using namespace web::http;
using namespace web::http::client;

TEST_CASE("Repository is working as expected", "[repository]") {

	SECTION("Creating the repository works fine") {
		REQUIRE_NOTHROW( repository::instance(true) );
	}

	SECTION("Can get the list of users") {
		repository& repo = repository::instance(true);
		auto repo_users = repo.get_users().get();
		REQUIRE((
			!repo_users.empty()
		));
	}
}
