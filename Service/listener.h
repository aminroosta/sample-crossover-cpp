#pragma once
#include <cpprest/http_listener.h>
#include "authorize.h"
#include <map>

namespace app {

	struct route_exception : public std::exception
	{
		route_exception(web::json::value _data, web::http::status_code _code = web::http::status_codes::BadRequest) :
			std::exception("app::route_exception"), code(_code) { 

				data[U("error")] = _data;
		}

		route_exception(utility::string_t message, web::http::status_code _code = web::http::status_codes::BadRequest) :
			std::exception("app::route_exception"), code(_code) { 

				data[U("error")] = web::json::value(message);
		}

		web::http::status_code code;
		web::json::value data;
	};

	struct listener {
		/* callbacks return json values. return_data_json CALLBACK(query_string_as_json, body_as_json) 
		 * If you need to indicate an error throw an instance of app::route_exception */
		typedef std::function<web::json::value(web::json::value, web::json::value)> callback_t;

		listener(web::uri address);
		pplx::task<void> open();
		pplx::task<void> close();
		
		/* order of registerations matters, routes are matched partially so register general routes at the end */
		void register_route(utility::string_t route, callback_t callback);

	private:
		std::vector<std::pair<utility::string_t, callback_t>> _routes;
		web::http::experimental::listener::http_listener _listener;
		pplx::task<void> dispatch(web::http::http_request req);
		authorize& auth;
	};
}