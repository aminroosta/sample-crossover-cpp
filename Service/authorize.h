#pragma once
#include <mutex>
#include <chrono>
#include <cpprest/http_client.h>

namespace app {

	struct userinfo {
		userinfo() = default;
		userinfo(utility::string_t _name)
			: entertime(std::chrono::system_clock::now()), name(_name) { }

		std::chrono::time_point<std::chrono::system_clock> entertime;
		utility::string_t name;
		utility::string_t cardid;
	};


	struct authorize {
		static authorize& instance();

		/* returns a token if successfull or an empty string if fails */
		pplx::task<utility::string_t> login(utility::string_t name, utility::string_t password);
		void logout(utility::string_t token);
		void logout_byname(utility::string_t name);

		/* the the card for a loged in user. returns ture if successfull false otherwise*/
		pplx::task<bool> set_card(utility::string_t token, utility::string_t id, utility::string_t pin);

		/* tokens are only valid for "timeout" milliseconds */
		bool validate_token(utility::string_t token);

		/* authorized routes access this */
		userinfo get_userinfo(utility::string_t token);
		userinfo get_userinfo(web::json::value token); /* json object with a token key*/

		std::chrono::milliseconds timeout; /* defaults to 10 minutes */
	private:
		authorize() : timeout(1000 * 60 * 10) { }
		authorize(const authorize&) = delete;
		authorize(authorize&&) = delete;

		utility::string_t _create_token();

		std::map<utility::string_t, userinfo> _users;
		std::mutex _users_mtx;
	};
}