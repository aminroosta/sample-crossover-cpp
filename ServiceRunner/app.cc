#include "../Service/utils.h"
#include "../Service/listener.h"
#include "../Service/app_routes.h"
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <memory>
#include <objbase.h>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using app::utils;
using std::cout;
using std::wcout;
using std::endl;

int main() {

	auto address = uri(U("http://127.0.0.1:3000"));
	app::listener listener(address);
	app::app_routes::register_all_routes(listener);

	listener.open().wait();

	wcout << U("Server is running at ") << address.to_string() << U(" ...") << endl;
	wcout << U("On your browser open http://127.0.0.1:3000 to test it.") << endl;
	char ch; std::cin >> ch >> ch;

	return 0;
} 