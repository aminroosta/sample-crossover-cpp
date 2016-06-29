#include"users.h"

using namespace app;

app::user::user(utility::string_t _name, utility::string_t _password, utility::size64_t _balance, utility::string_t _currency)
	: name(_name), password(_password), balance(_balance), currency(_currency) { }

app::transaction::transaction(utility::string_t _type, utility::size64_t _amount, utility::string_t _date)
	: type(_type), amount(_amount), date(_date) { }

app::card::card(utility::string_t _id, utility::string_t _pin)
	: id(_id), pin(_pin) { }

std::vector<user> user::create_initial_users()
{
	auto jack = user(U("jack"), U("jackpassword"), 1000, U("USD"));
	auto bob = user(U("bob"), U("bob-password"), 2100, U("EUR"));

	jack.cards.push_back(card(U("1234"), U("1234")));
	bob.cards.push_back(card(U("1234"), U("1234")));

	jack.transactions.push_back(transaction(U("withdraw"), 10, U("2016-06-28")));
	bob.transactions.push_back(transaction(U("withdraw"), 12, U("2016-06-29")));

	return std::vector<user> {
		std::move(jack), std::move(bob)
	};
}
