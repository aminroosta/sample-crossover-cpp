#include "repository.h"
#include <stdexcept>

using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace utility;
using std::move;
using web::json::value;

repository::repository(http_client _client)
	: client(move(_client)) { }

/* get the url from config file */
repository::repository()
	: client(http_client(get_app_config().get()[U("url")].as_string())) { }

pplx::task<value> repository::authorize(string_t name, string_t password) {
	return
		client.request(methods::GET,
			uri_builder(U("/authorize"))
			.append_query(U("name"), name)
			.append_query(U("password"), password)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
	}).then([this](pplx::task<value> pervTask) {
		value ret;
		try {
			ret = pervTask.get();
			if (ret.has_field(U("token")) & ret[U("token")].is_string())
				this->token = ret[U("token")].as_string();
		}
		catch (std::exception& e) {
			auto error = utility::conversions::to_string_t(e.what());
			ret[U("error")] = value(error);
		}
		return ret;
	});
}
