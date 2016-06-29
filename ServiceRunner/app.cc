#include "../Service/utils.h"
#include <cpprest/json.h>
#include <memory>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using app::utils;
using std::cout;
using std::endl;


int main() {

	pplx::task<void> tsk([]() {
		cout << "hey you!" << endl;
	});

	auto tsk2 = tsk.then([]() {
		cout << "tsk2" << endl;
	});

	auto tsk3 = tsk.then([]() {
		cout << "tsk3" << endl;
	});

	tsk2.wait();
	tsk3.wait();

	//http_client client(U("http://bing.com"));
	//uri_builder builder(U("/search"));
	//builder.append_query(U("q"), U("cpprestsdk github"));
	//auto requestTask = client
	//	.request(methods::GET, builder.to_string())
	//	.then([](http_response response) {
	//		return utils::write_file(response.body(), U("res.html"));
	//	});

	//try {
	//	requestTask.wait();
	//}
	//catch (const std::exception& e) {
	//	printf("Error: %s\n", e.what());
	//}
	return 0;
} 