#pragma once
#include <cpprest\json.h>
#include <vector>
#include "serializeable.h"

namespace app {

	struct card {
		card(utility::string_t id, utility::string_t pin);

		utility::string_t id;
		utility::string_t pin;

		bool operator==(const card& other) const;
	};

	struct transaction {
		transaction(utility::string_t type, utility::size64_t amount, utility::string_t date);

		utility::string_t type;
		utility::size64_t amount;
		utility::string_t date;

		bool operator==(const transaction& other) const;
	};

	struct user {
		user(utility::string_t name, utility::string_t password, utility::size64_t balance, utility::string_t currency);

		utility::string_t name;
		utility::string_t password;
		utility::size64_t balance;
		utility::string_t currency;

		std::vector<card> cards;
		std::vector<transaction> transactions;

		bool operator==(const user& other) const;

		/* create a list of dummy users */
		static std::vector<user> create_initial_users();
	};


	/*--------------------------- Serilaizeable specializations for each entity class we have --------------------------------*/
	template<>
	struct serializeable<card> {

		static web::json::value serialize(const card& card) {
			web::json::value value;
			value[U("id")] = web::json::value(card.id);
			value[U("pin")] = web::json::value(card.pin);
			return value;
		}

		static card parse(web::json::value value) {
			return card(
				value[U("id")].as_string(),
				value[U("pin")].as_string()
			);
		}
	};

	template<>
	struct serializeable<transaction> {

		static web::json::value serialize(const transaction& tran) {
			web::json::value value;
			value[U("type")] = web::json::value(tran.type);
			value[U("amount")] = web::json::value(tran.amount);
			value[U("date")] = web::json::value(tran.date);
			return value;
		}

		static transaction parse(web::json::value value) {
			return transaction(
				value[U("type")].as_string(),
				value[U("amount")].as_integer(),
				value[U("date")].as_string()
			);
		}
	};

	template<typename T>
	struct serializeable<std::vector<T>> {

		static web::json::value serialize(const std::vector<T>& vec) {
			std::vector<web::json::value> value_vec;
			for (auto& elem : vec)
				value_vec.push_back(serializeable<T>::serialize(elem));
			return web::json::value::array(value_vec);
		}

		static std::vector<T> parse(web::json::value value) {
			auto value_vec = value.as_array();
			std::vector<T> vec;
			for (auto& elem : value_vec)
				vec.push_back(serializeable<T>::parse(elem));
			return vec;
		}
	};

	template<>
	struct serializeable<user> {

		static web::json::value serialize(const user& usr) {
			web::json::value value;
			value[U("name")] = web::json::value(usr.name);
			value[U("password")] = web::json::value(usr.password);
			value[U("balance")] = web::json::value(usr.balance);
			value[U("currency")] = web::json::value(usr.currency);

			value[U("cards")] = serializeable<std::vector<card>>::serialize(usr.cards);
			value[U("transactions")] = serializeable<std::vector<transaction>>::serialize(usr.transactions);

			return value;
		}

		static user parse(web::json::value value) {
			user usr (
				value[U("name")].as_string(),
				value[U("password")].as_string(),
				value[U("balance")].as_integer(),
				value[U("currency")].as_string()
			);

			usr.cards = serializeable<decltype(usr.cards)>::parse(value[U("cards")]);
			usr.transactions = serializeable<decltype(usr.transactions)>::parse(value[U("transactions")]);

			return usr;
		}
	};
}