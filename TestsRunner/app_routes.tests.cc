#include "../Service/listener.h"
#include "../Service/app_routes.h"
#include "../Service/repository.h"
#include <cpprest/http_client.h>
#include "catch.hpp"

using namespace app;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;
using namespace utility;

TEST_CASE("all app routes work just fine", "[app_routes]") {

	auto address = uri(U("http://127.0.0.1:3000"));
	app::listener listener(address);
	app::app_routes::register_all_routes(listener);

	/* run the server */
	REQUIRE_NOTHROW(listener.open().wait());

	http_client client(address);
	auto req = client.request(methods::GET, U("/authorize?name=unknown_username"))
		.then([](http_response res) { return res.extract_json();  });

	/* unknow username is not logged in*/
	REQUIRE(req.get().has_field(U("error")));

	auto users = repository::instance(true).get_users().get();
	auto& user = users[0];

	req = client.request(
		methods::GET,
		uri_builder(U("/authorize")).append_query(U("name"), user.name).append_query(U("password"), user.password).to_string()
	).then([](http_response res) { return res.extract_json();  });



	/* login works just fine */
	REQUIRE(req.get().has_field(U("token")));
	auto token = req.get()[U("token")].as_string();
	REQUIRE(!token.empty());

	auto& crd = user.cards[0];

	req = client.request(
		methods::GET,
		uri_builder(U("/set/card"))
		.append_query(U("token"), token)
		.append_query(U("id"), crd.id)
		.append_query(U("pin"), crd.pin)
		.to_string()
	).then([](http_response res) { return res.extract_json();  });

	/* /set/card works fine */
	REQUIRE(req.get().has_field(U("status")));

	req = client.request(
		methods::GET,
		uri_builder(U("/balance/check"))
		.append_query(U("token"), token)
		.to_string()
	).then([](http_response res) { return res.extract_json();  });

	/* /balance/check works fine */
	REQUIRE(req.get().has_field(U("balance")));

	req = client.request(
		methods::GET,
		uri_builder(U("/cash/withdraw"))
		.append_query(U("token"), token)
		.append_query(U("amount"), 12)
		.to_string()
	).then([](http_response res) { return res.extract_json();  });

	/* /cash/withdraw works fine */
	REQUIRE(req.get().has_field(U("balance")));

	req = client.request(
		methods::GET,
		uri_builder(U("/pin/change"))
		.append_query(U("token"), token)
		.append_query(U("oldpin"), crd.pin)
		.append_query(U("newpin"), "2323")
		.to_string()
	).then([](http_response res) { return res.extract_json();  });

	/* /pin/change works fine */
	REQUIRE(req.get().has_field(U("card")));


	req = client.request(
		methods::GET,
		uri_builder(U("/mini/statement"))
		.append_query(U("token"), token)
		.to_string()
	).then([](http_response res) { return res.extract_json();  });

	/* /mini/statement works fine */
	REQUIRE(req.get().has_field(U("statements")));

	listener.close().wait();
}
