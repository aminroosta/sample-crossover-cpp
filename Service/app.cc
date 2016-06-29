#include "utils.hpp"
#include <memory>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using app::utils;


int main() {

	auto readTask = utils::read_file(U("config.json")).then([](std::string content) {
		std::cout << content << std::endl;
	});

	http_client client(U("http://bing.com"));
	uri_builder builder(U("/search"));
	builder.append_query(U("q"), U("cpprestsdk github"));
	auto requestTask = client
		.request(methods::GET, builder.to_string())
		.then([](http_response response) {
			return utils::write_file(response.body(), U("res.html"));
		});

	try {
		requestTask.wait();
		readTask.wait();
	}
	catch (const std::exception& e) {
		printf("Error: %s\n", e.what());
	}
	return 0;
} 