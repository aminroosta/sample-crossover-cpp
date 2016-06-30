#include "../Service/utils.h"
#include "../Service/listener.h"
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

	GUID guid;
	CoCreateGuid(&guid);

	printf("%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	//auto address = uri(U("http://127.0.0.1:3000"));
	//app::listener listener(address);

	//listener.register_route(U("/"), [](json::value params, json::value body) {
	//	wcout << params << endl << body << endl;
	//	return json::value(U("OK!"));
	//});

	//listener.open().wait();
	//wcout << U("Server is running at ") << address.to_string() << endl;

	//char ch; std::cin >> ch;

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