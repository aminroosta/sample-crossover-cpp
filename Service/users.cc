#include"users.h"

using namespace app;
using std::vector;

template<typename T>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
	if (lhs.size() != rhs.size()) return false;
	for (size_t inx = 0; inx < lhs.size(); ++inx)
		if (!(lhs[inx] == rhs[inx]))
			return false;
	return true;
}

app::user::user(utility::string_t _name, utility::string_t _password, utility::size64_t _balance, utility::string_t _currency)
	: name(_name), password(_password), balance(_balance), currency(_currency) { }

app::transaction::transaction(utility::string_t _type, utility::size64_t _amount, utility::string_t _date)
	: type(_type), amount(_amount), date(_date) { }

bool app::transaction::operator==(const transaction & other) const {
	return this->amount == other.amount && this->date == other.date && this->type == other.type;
}

app::card::card(utility::string_t _id, utility::string_t _pin)
	: id(_id), pin(_pin) { }

bool app::card::operator==(const card & other) const {
	return this->id == other.id && this->pin == other.pin;
}

bool app::user::operator==(const user & other) const {
	return this->balance == other.balance && this->currency == other.currency &&
		this->name == other.name && this->password == other.password &&
		this->cards == other.cards && this->transactions == other.transactions;
}

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
