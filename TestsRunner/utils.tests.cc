#include "../Service/utils.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace app;
using namespace concurrency::streams;
using namespace utility;
using namespace web::http;
using namespace web::http::client;

TEST_CASE("Can read an write into files", "[utils]") {

	const string_t content = U("12345");

	SECTION("Wrting to a file") {
		auto writeTask = utils::write_file(U("12345"), U("write-task-test.txt"));
		REQUIRE_NOTHROW(writeTask.get());
	}

	SECTION("Wrting from a file") {
		auto readTask = utils::read_file(U("write-task-test.txt"));
		REQUIRE(readTask.get() == "12345");
	}
}
