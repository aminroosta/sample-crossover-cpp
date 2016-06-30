#pragma once
#include <cpprest/http_listener.h>
#include "authorize.h"
#include <map>

#define API_HELP_MESSAGE "\n\nYou can use the follwoing apis: \n" \
						 "/authorize?name=name&password=apssword    - retruns a token that will last for 10 minutes.\n" \
						 "/set/card?token=toke&id=id&pin=pin        - sets the active user card.\n" \
						 "/balance/check?token=token                - returns account balance info.\n" \
						 "/mini/statement?token=token               - retruns list of statements.\n" \
						 "/cash/withdraw?token=token&amount=12              - withdraw money from your account.\n" \
						 "/pin/change?token=token&oldpin=oldpin&newpin=new  - change active card pin number.\n" \
						 "\n\nCheck users.json file on the server to see the list of  registered users."

namespace app {


	struct route_exception : public std::exception
	{
		route_exception(web::json::value _data, web::http::status_code _code = web::http::status_codes::BadRequest) :
			std::exception("app::route_exception"), code(_code) { 

				data[U("error")] = _data;
				data[U("help")] = web::json::value(U(API_HELP_MESSAGE));
		}

		route_exception(utility::string_t message, web::http::status_code _code = web::http::status_codes::BadRequest) :
			std::exception("app::route_exception"), code(_code) { 

				data[U("error")] = web::json::value(message);
				data[U("help")] = web::json::value(U(API_HELP_MESSAGE));
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