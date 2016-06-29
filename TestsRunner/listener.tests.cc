#include "../Service/listener.h"
#include <cpprest/http_client.h>
#include "catch.hpp"

using namespace app;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace utility;

TEST_CASE("http listener works as expected", "[listener]") {

	auto address = uri(U("http://127.0.0.1:3000"));
	app::listener listener(address);

	json::value params, body;
	json::value result = json::value(U("OK"));
	json::value sent_body = json::value(U("sent body"));
	json::value sent_params;
	sent_params[U("A")] = json::value(U("a"));


	listener.register_route(U("/my/route"), [&](json::value _params, json::value _body) {
		params = _params;
		body = _body;
		return result;
	});
	
	/* run the server */
	REQUIRE_NOTHROW(listener.open().wait());

	http_client client(address);
	auto req = client.request(methods::POST, U("/my/route?A=a"), sent_body);

	REQUIRE_NOTHROW(req.wait());

	auto returned_result = req.get().extract_json().get();

	REQUIRE((
		returned_result == result &&
		sent_body == body &&
		sent_params == params
		));

	REQUIRE_NOTHROW(listener.close().wait());
}
