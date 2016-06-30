#pragma once
#include "listener.h"

namespace app {
	struct app_routes {
		static void register_all_routes(listener& listener);

		static web::json::value set_card(web::json::value params, web::json::value body);
		static web::json::value balance_check(web::json::value params, web::json::value body);
		static web::json::value cash_withdraw(web::json::value params, web::json::value body);
		static web::json::value pin_change(web::json::value params, web::json::value body);
		static web::json::value mini_statement(web::json::value params, web::json::value body);
	};
};