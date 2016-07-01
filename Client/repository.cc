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

pplx::task<web::json::value> repository::set_card(utility::string_t id, utility::string_t pin)
{
	return
		client.request(methods::GET,
			uri_builder(U("/set/card"))
			.append_query(U("token"), token)
			.append_query(U("id"), id)
			.append_query(U("pin"), pin)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
		}).then([this](pplx::task<value> pervTask) {
			value ret;
			try {
				ret = pervTask.get();
			}
			catch (std::exception& e) {
				auto error = utility::conversions::to_string_t(e.what());
				ret[U("error")] = value(error);
			}
			return ret;
		});
}

pplx::task<web::json::value> repository::balance_check() {
	return
		client.request(methods::GET,
			uri_builder(U("/balance/check"))
			.append_query(U("token"), token)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
		}).then([this](pplx::task<value> pervTask) {
			value ret;
			try {
				ret = pervTask.get();
			}
			catch (std::exception& e) {
				auto error = utility::conversions::to_string_t(e.what());
				ret[U("error")] = value(error);
			}
			return ret;
		});
}

pplx::task<web::json::value> repository::cash_withdraw(int amount) {
	return
		client.request(methods::GET,
			uri_builder(U("/cash/withdraw"))
			.append_query(U("token"), token)
			.append_query(U("amount"), amount)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
		}).then([this](pplx::task<value> pervTask) {
			value ret;
			try {
				ret = pervTask.get();
			}
			catch (std::exception& e) {
				auto error = utility::conversions::to_string_t(e.what());
				ret[U("error")] = value(error);
			}
			return ret;
		});
}

pplx::task<web::json::value> repository::pin_change(utility::string_t oldpin, utility::string_t newpin) {
	return
		client.request(methods::GET,
			uri_builder(U("/pin/change"))
			.append_query(U("token"), token)
			.append_query(U("oldpin"), oldpin)
			.append_query(U("newpin"), newpin)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
		}).then([this](pplx::task<value> pervTask) {
			value ret;
			try {
				ret = pervTask.get();
			}
			catch (std::exception& e) {
				auto error = utility::conversions::to_string_t(e.what());
				ret[U("error")] = value(error);
			}
			return ret;
		});
}

pplx::task<web::json::value> repository::mini_statement() {
	return
		client.request(methods::GET,
			uri_builder(U("/mini/statement"))
			.append_query(U("token"), token)
			.to_string()
		).then([](http_response response) {
			return response.extract_json();
		}).then([this](pplx::task<value> pervTask) {
			value ret;
			try {
				ret = pervTask.get();
			}
			catch (std::exception& e) {
				auto error = utility::conversions::to_string_t(e.what());
				ret[U("error")] = value(error);
			}
			return ret;
		});
}
