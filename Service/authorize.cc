#include "authorize.h"
#include <objbase.h>


using namespace utility;
using namespace app;
using namespace std::chrono;
using std::vector;

authorize& authorize::instance() {
	static authorize _instance;
	return _instance;
}

pplx::task<string_t> authorize::login(string_t name, string_t password) {
	return repository::instance().get_users().then([=](vector<user> users) {
		for (auto&& user : users)
			if (user.name == name && user.password == password) {
				return user.name;
			}
		return string_t();
	}).then([this](string_t username) {
		if (username.empty())
			return string_t(); // return an empty token

		logout_byname(username);

		/* add the new user */
		{
			std::lock_guard<std::mutex> guard(this->_users_mtx);
			auto token = this->_create_token();
			this->_users.emplace(token, userinfo(username));
			return token;
		}
	});
}

void authorize::logout(utility::string_t token) {
	std::lock_guard<std::mutex> guard(_users_mtx);
	_users.erase(token);
}

void authorize::logout_byname(utility::string_t name) {
	string_t perv_token;
	for(auto&& u : this->_users)
		if (u.second.name == name) {
			perv_token = u.first;
			break;
		}
	logout(perv_token);
}

pplx::task<bool> authorize::set_card(utility::string_t token, utility::string_t id, utility::string_t pin) {
	if (_users.count(token) == 0)
		return pplx::task<bool>([] { return false;  });

	userinfo& info = _users[token];
	return repository::instance().get_users().then([&info, id, pin](vector<user> users) {
		for(auto&& u : users)
			if (u.name == info.name) {
				for(auto&& crd : u.cards) 
					if (crd.id == id && crd.pin == pin) {
						info.cardid = id;
						return true;
					}
			}
		return false;
	});
}

bool authorize::validate_token(utility::string_t token) {

	if(_users.count(token) == 0)
		return false;
	userinfo& info = _users[token];
	
	auto now = system_clock::now();
	auto difference = duration_cast<milliseconds>(now - info.entertime);

	if (difference > timeout) { 
		std::lock_guard<std::mutex> guard(_users_mtx); /* thread safe */
		_users.erase(token);
		return false;
	}

	return true;
}

string_t authorize::_create_token() {
	GUID guid;
	CoCreateGuid(&guid); /* use windows function to create a guid which we will use as a token */

	char guidstr[36 + 1];
	sprintf(guidstr, "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

	return utility::conversions::to_string_t(guidstr);
}
