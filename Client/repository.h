#pragma once
#include "utils.h"
#include <cpprest/http_client.h>
#include <cpprest/json.h>

struct repository {
	repository(web::http::client::http_client client);
	repository();
	
	pplx::task<web::json::value> authorize(utility::string_t name, utility::string_t password);
	pplx::task<web::json::value> set_card(utility::string_t id, utility::string_t pin);
	pplx::task<web::json::value> balance_check();
	pplx::task<web::json::value> cash_withdraw(int amount);
	pplx::task<web::json::value> pin_change(utility::string_t oldpin, utility::string_t newpin);
	pplx::task<web::json::value> mini_statement();

private:
	utility::string_t token;
	web::http::client::http_client client;
};