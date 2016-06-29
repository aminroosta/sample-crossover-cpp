#pragma once
#include <cpprest\json.h>
#include <vector>

namespace app {

	struct card {
		card(utility::string_t id, utility::string_t pin);

		utility::string_t id;
		utility::string_t pin;
	};

	struct transaction {
		transaction(utility::string_t type, utility::size64_t amount, utility::string_t date);

		utility::string_t type;
		utility::size64_t amount;
		utility::string_t date;
	};

	struct user {
		user(utility::string_t name, utility::string_t password, utility::size64_t balance, utility::string_t currency);

		utility::string_t name;
		utility::string_t password;
		utility::size64_t balance;
		utility::string_t currency;

		std::vector<card> cards;
		std::vector<transaction> transactions;

		/* create a list of dummy users */
		static std::vector<user> create_initial_users();
	};

}