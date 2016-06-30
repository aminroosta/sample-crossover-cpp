#include "app_routes.h"
#include "authorize.h"
#include "repository.h"
#include "utils.h"
#include <chrono>
#include <vector>

using namespace app;
using namespace utility;
using web::http::status_codes;
using web::json::value;

void app::app_routes::register_all_routes(listener& listener) {
	listener.register_route(U("/set/card"), set_card);
	listener.register_route(U("/balance/check"), balance_check);
	listener.register_route(U("/cash/withdraw"), cash_withdraw);
	listener.register_route(U("/pin/change"), pin_change);
	listener.register_route(U("/mini/statement"), mini_statement);
}

/*----------------------- helper functions -------------------------*/
static user& finduser(std::vector<user>& users, string_t name) {
	for (auto&& u : users)
		if (u.name == name)
			return u;
	throw route_exception(U("ERROR: user not found"));
}

static card& findcard(std::vector<card>& cards, string_t cardid) {
	for (auto&& c : cards)
		if (c.id == cardid)
			return c;
	throw route_exception(U("ERROR: card not found"));
};

/*--------------------- routes ------------------------------------*/

web::json::value app::app_routes::set_card(web::json::value params, web::json::value body) {
	if(!params[U("id")].is_string() || !params[U("pin")].is_string())
		throw route_exception(U("ERROR: invalid parameters use /set/card?token=token&id=id&pin=pin"));
	auto token = params[U("token")].as_string();
	auto id = params[U("id")].as_string();
	auto pin = params[U("pin")].as_string();

	bool ok = authorize::instance().set_card(token, id, pin).get();
	if(!ok)
		throw route_exception(U("ERROR: no such card found"));
	value ret;
	ret[U("status")] = value(U("OK"));
	return ret;
}

value app::app_routes::balance_check(value params, value body) {
	userinfo info = authorize::instance().get_userinfo(params);
	auto users = repository::instance().get_users().get();
	auto& user = finduser(users, info.name);

	value ret;
	ret[U("name")] = value(user.name);
	ret[U("currency")] = value(user.currency);
	ret[U("balance")] = value(user.balance);
	return ret;
}

value app::app_routes::cash_withdraw(value params, value body) {
	userinfo info = authorize::instance().get_userinfo(params);
	auto users = repository::instance().get_users().get();
	auto& user = finduser(users, info.name);
	
	int amount = 0;
	try {
		/*I'm not sure why but it will be something like "\"12\"" */
		auto str = params[U("amount")].to_string();
		/* remove the double quotes (") */
		str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
		amount = std::stoi(str);
	}
	catch (...) {
		throw route_exception(U("ERROR: invalid parameters use /cash/withdraw?token=token&amount=amount"));
	}

	if (user.balance < amount)
		throw route_exception(U("ERROR: balance is not enough"));

	user.balance -= amount;

	auto now = std::chrono::system_clock::now();
	user.transactions.push_back(transaction(U("withdraw"), amount, utils::time_point_to_string(now)));

	repository::instance().set_users(users).get();

	value ret;
	ret[U("name")] = value(user.name);
	ret[U("currency")] = value(user.currency);
	ret[U("balance")] = value(user.balance);
	return ret;
}

value app::app_routes::pin_change(value params, value body) {
	userinfo info = authorize::instance().get_userinfo(params);
	auto users = repository::instance().get_users().get();
	auto& user = finduser(users, info.name);
	
	if(!params[U("oldpin")].is_string() || !params[U("newpin")].is_string())
		throw route_exception(U("ERROR: invalid parameters use /pin/change?token=token&oldpin=oldpin&newpin=newpin"));

	auto oldpin = params[U("oldpin")].as_string();
	auto newpin = params[U("newpin")].as_string();

	if(info.cardid.empty())
		throw route_exception(U("ERROR: access denied, no card has been set"));

	card& crd = findcard(user.cards, info.cardid);
	if (crd.pin != oldpin)
		throw route_exception(U("ERROR: old pin is incorrect"));
	if (newpin.length() != 4)
		throw route_exception(U("ERROR: new pin must be exactly 4 digits"));

	crd.pin = newpin;
	repository::instance().set_users(users).get();

	value ret;
	ret[U("status")] = value(U("OK"));
	ret[U("card")] = serializeable<card>::serialize(crd);
	return ret;
}

value app::app_routes::mini_statement(value params, value body) {
	userinfo info = authorize::instance().get_userinfo(params);
	auto users = repository::instance().get_users().get();
	auto& user = finduser(users, info.name);

	value ret;
	ret[U("status")] = value(U("OK"));
	ret[U("statements")] = serializeable<std::vector<transaction>>::serialize(user.transactions);
	return ret;
} 